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

void save_command(cmd_buff_t *cmd) {
    char *temp_buff = cmd->_cmd_buffer;
    char word[SH_CMD_MAX];
    int index = 0;
    int quotes_checker = 0;  
    while (*temp_buff != '\0') {
        if (*temp_buff == '"') {
            quotes_checker++;
            temp_buff++;
            continue;
        }
        if (*temp_buff == ' ' && quotes_checker % 2 == 0) {
            if (index > 0) {
                word[index] = '\0';
                cmd->argv[cmd->argc] = strdup(word);
                cmd->argc++;
                index = 0;
            }
        } else {
            word[index] = *temp_buff;
            index++;
        }

        temp_buff++;
    }
    if (index > 0) {
        word[index] = '\0';
        cmd->argv[cmd->argc] = strdup(word);
        if (cmd->argv[cmd->argc] == NULL) {
            exit(ERR_MEMORY);
        }
        cmd->argc++;
    }

    cmd->argv[cmd->argc] = NULL; 
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


int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd) {
	if (strlen(cmd_line) == 0) {
        return WARN_NO_CMDS;
    }
    char *cmd_buff = trim_spaces(cmd_line);
    char *temp_buff = cmd_buff;
    char* formatted_buff = cmd->_cmd_buffer;
    char prev_char = '\0';
    int argument_count = 0;
    int quotes_check = 0;

    while (*temp_buff != '\0') {
        if (*temp_buff == '"') {
            quotes_check++;
        }
        if ((*temp_buff != ' ' || prev_char != ' ') || (*temp_buff != ' ' || quotes_check % 2 != 0)) {
            *formatted_buff = *temp_buff;
            formatted_buff++;
        }
        else if (*temp_buff == ' ' && prev_char != ' ' && quotes_check % 2 == 0) {
            argument_count++;
        }

        prev_char = *temp_buff;
        temp_buff++;
    }
    *formatted_buff = '\0';
    if(argument_count > CMD_ARGV_MAX){
    	return ERR_TOO_MANY_COMMANDS;
    }
    cmd->argc = argument_count;
    save_command(cmd);
    if(strcmp(cmd->argv[0], "cd") == 0){
		return handle_cd(cmd);
    }
    else{
		pid_t pid = fork();
    	if (pid == 0) {
        	execvp(cmd->argv[0], cmd->argv);
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
    return OK;
}

int exec_local_cmd_loop()
{
    char *cmd_buff;
    int rc = 0;
    cmd_buff_t *cmd = (cmd_buff_t*)malloc(sizeof(cmd_buff_t));

        cmd->_cmd_buffer = (char *)malloc(SH_CMD_MAX * sizeof(char));
        cmd_buff = (char *)malloc(SH_CMD_MAX * sizeof(char));
        for (int i = 0; i < CMD_ARGV_MAX; i++) {
        cmd->argv[i] = malloc(ARG_MAX * sizeof(char));
        }
    // TODO IMPLEMENT MAIN LOOP
    while(1){
        printf("%s", SH_PROMPT);
                if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL){
                        printf("\n");
                        break;
                }
                //remove the trailing \n from cmd_buff
                cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
				
				if (strcmp(cmd_buff, EXIT_CMD) == 0) {
        			free(cmd_buff);
        			for (int i = 0; i < CMD_ARGV_MAX; i++) {
        				free(cmd->argv[i]);
    				}
    				free(cmd->_cmd_buffer);
    				free(cmd);
            		exit(0);
        		}
                //IMPLEMENT THE REST OF THE REQUIREMENTS
                rc = build_cmd_buff(cmd_buff, cmd);
                if(rc == WARN_NO_CMDS){
					printf(CMD_WARN_NO_CMD);
                }
                /*
                if(rc == ERR_EXEC_CMD){
					printf(CMD_ERR_EXECUTE);
                }*/

        }
    free(cmd_buff);
    for (int i = 0; i < CMD_ARGV_MAX; i++) {
        free(cmd->argv[i]);
    }
    free(cmd->_cmd_buffer);
    free(cmd);
    return OK;
}
