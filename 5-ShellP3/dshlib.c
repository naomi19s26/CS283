#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "dshlib.h"

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
  
int handle_cd(cmd_buff_t *cmd) {
    if (cmd->argc == 1) {
        return OK;
    } else if (cmd->argc == 2) {
        if (chdir(cmd->argv[1]) != 0) {
            return ERR_EXEC_CMD;
        }
    } else {
        return ERR_EXEC_CMD;
    }
    return OK;
}

char *trim_spaces(char *string) {
    char *end;

    while (*string == SPACE_CHAR) {
        string++;
    }
    if (*string == 0) {
        return string;
    }

    end = string + strlen(string) - 1;
    while (end > string && *end == SPACE_CHAR) {
        end--;
    }

    *(end + 1) = '\0';

    return string;
}

int save_command(command_list_t *clist, int index) {
    char *temp_buff = clist->commands[index]._cmd_buffer;
    int arg_size = 0;
    char word[ARG_MAX+1];
    int index_word = 0;
    int quotes_checker = 0;

    while (*temp_buff != '\0') {
        if (*temp_buff == '\n') {
            temp_buff++; 
            continue;
        }
        if (*temp_buff == '"') {
            quotes_checker++;
            temp_buff++;
            continue;
        }
        if (*temp_buff == ' ' && quotes_checker % 2 == 0) {
            if(arg_size >= ARG_MAX){
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }
            if (index_word > 0) {
                word[index_word] = '\0';
                clist->commands[index].argv[clist->commands[index].argc] = strdup(word);
                if (clist->commands[index].argv[clist->commands[index].argc] == NULL) {
                    exit(ERR_MEMORY);
                }
                clist->commands[index].argc++;
                index_word = 0;
                arg_size = 0;
            }
        } else {
            word[index_word++] = *temp_buff;
            arg_size++;
        }
        temp_buff++;
    }

    if (index_word > 0) {
        word[index_word] = '\0';
        clist->commands[index].argv[clist->commands[index].argc] = strdup(word);
        if (clist->commands[index].argv[clist->commands[index].argc] == NULL) {
            exit(ERR_MEMORY);
        }
        clist->commands[index].argc++;
    }
    clist->commands[index].argv[clist->commands[index].argc] = NULL;
    return OK;
}

int build_cmd_list(char *cmd_line, command_list_t *clist) {
    if (strlen(cmd_line) == 0) {
        return WARN_NO_CMDS;
    }

    int command_count = 1;
    char *temp = cmd_line;
    while (*temp != '\0') {
        if (*temp == PIPE_CHAR) {
            command_count++;
        }
        temp++;
    }
    if (command_count > CMD_MAX) {
        return ERR_TOO_MANY_COMMANDS;
    }
    clist->num = 0;

    char *cmd_line_copy = strdup(cmd_line);
    if (cmd_line_copy == NULL) {
        return ERR_MEMORY;
    }

    char *command = strtok(cmd_line_copy, PIPE_STRING);
    int index = 0;
    while (command != NULL) {
        char *cmd_buff = trim_spaces(command);
        clist->commands[index]._cmd_buffer = (char *)malloc(SH_CMD_MAX * sizeof(char));
        if (clist->commands[index]._cmd_buffer == NULL) {
            free(cmd_line_copy);
            return ERR_MEMORY;
        }
        strncpy(clist->commands[index]._cmd_buffer, cmd_buff, SH_CMD_MAX - 1);
        clist->commands[index]._cmd_buffer[SH_CMD_MAX - 1] = '\0';

        clist->commands[index].argc = 0;
        if(save_command(clist, index) != OK){
            return save_command(clist, index);
        }

        command = strtok(NULL, PIPE_STRING);
        index++;
    }
    clist->num = index;
    free(cmd_line_copy);
    return OK;
}


int execute_piped_commands(command_list_t *clist) {
    if (clist->num < 1) {
        return ERR_EXEC_CMD;
    }
    if (clist->num == 1 && strcmp(clist->commands[0].argv[0], "cd") == 0) {
        return handle_cd(&clist->commands[0]);
    }
    else if (clist->num == 1){
        pid_t pid = fork();
        if (pid == 0) {
            execvp(clist->commands[0].argv[0], clist->commands[0].argv);
            exit(ERR_EXEC_CMD);
        } else if (pid > 0) {
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                return ERR_EXEC_CMD; 
            }
        } else {
            return ERR_EXEC_CMD;
        }
    }
    else {
        int pipes[clist->num - 1][2];
        pid_t pids[clist->num];
    
        for (int i = 0; i < clist->num -1; i++) {
            if (pipe(pipes[i]) == -1) {
                return ERR_EXEC_CMD;
            }
        }
        for (int i = 0; i < clist->num; i++) {
            pids[i] = fork();
            if (pids[i] == -1) {
                return ERR_EXEC_CMD;
            }
            if (pids[i] == 0) { 
                if (i > 0) {
                    dup2(pipes[i - 1][0], STDIN_FILENO);
                    close(pipes[i - 1][1]);
                }
                if (i < clist->num - 1) {
                    dup2(pipes[i][1], STDOUT_FILENO);
                    close(pipes[i][0]);
                }
    
                for (int j = 0; j < clist->num - 1; j++) {
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }
                execvp(clist->commands[i].argv[0], clist->commands[i].argv);
                exit(ERR_EXEC_CMD); 
            }
        }
        for (int i = 0; i < clist->num - 1; i++) {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }
        for (int i = 0; i < clist->num; i++) {
            int status;
            if (waitpid(pids[i], &status, 0) == -1) {
                return ERR_EXEC_CMD;
            }
            if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                return ERR_EXEC_CMD;
            }
        }
    }
    return OK;
}

int exec_local_cmd_loop() {
    char cmd_line[SH_CMD_MAX];
    command_list_t clist;
    int rc;

    while (1) {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_line, ARG_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }
        cmd_line[strcspn(cmd_line, "\n")] = '\0';

        if (strcmp(cmd_line, EXIT_CMD) == 0) {
            return OK;
        }
        rc = build_cmd_list(cmd_line, &clist);
        if (rc == WARN_NO_CMDS) {
            printf("%s", CMD_WARN_NO_CMD);
            continue;
        } else if (rc == ERR_TOO_MANY_COMMANDS) {
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            continue;
        }
        else if (rc == ERR_CMD_OR_ARGS_TOO_BIG){
            continue;
        }
        else{
            execute_piped_commands(&clist);
        }
        
    }
    return OK;
}
