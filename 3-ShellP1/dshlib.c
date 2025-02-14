#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
void print_dragon() {
    printf("                                                                        @%%%%                        \n");
    printf("                                                                     %%%%%%                          \n");
    printf("                                                                    %%%%%%                           \n");
    printf("                                                                 % %%%%%%%           @               \n");
    printf("                                                                %%%%%%%%%%        %%%%%%%           \n");
    printf("                                       %%%%%%%  %%%%@         %%%%%%%%%%%%@    %%%%%%  @%%%%        \n");
    printf("                                  %%%%%%%%%%%%%%%%%%%%%%      %%%%%%%%%%%%%%%%%%%%%%%%%%%%          \n");
    printf("                                %%%%%%%%%%%%%%%%%%%%%%%%%%   %%%%%%%%%%%% %%%%%%%%%%%%%%%           \n");
    printf("                               %%%%%%%%%%%%%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%%%%     %%%            \n");
    printf("                             %%%%%%%%%%%%%%%%%%%%%%%%%%%%@ @%%%%%%%%%%%%%%%%%%        %%            \n");
    printf("                            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%%%%%%%                 \n");
    printf("                            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%               \n");
    printf("                            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%@%%%%%%@               \n");
    printf("      %%%%%%%%@           %%%%%%%%%%%%%%%%        %%%%%%%%%%%%%%%%%%%%%%%%%%      %%                \n");
    printf("    %%%%%%%%%%%%%         %%@%%%%%%%%%%%%           %%%%%%%%%%% %%%%%%%%%%%%      @%                \n");
    printf("  %%%%%%%%%%   %%%        %%%%%%%%%%%%%%            %%%%%%%%%%%%%%%%%%%%%%%%                        \n");
    printf(" %%%%%%%%%       %         %%%%%%%%%%%%%             %%%%%%%%%%%%@%%%%%%%%%%%                        \n");
    printf("%%%%%%%%%@                % %%%%%%%%%%%%%            @%%%%%%%%%%%%%%%%%%%%%%%%%                      \n");
    printf("%%%%%%%%@                 %%@%%%%%%%%%%%%            @%%%%%%%%%%%%%%%%%%%%%%%%%%%%                   \n");
    printf("%%%%%%%@                   %%%%%%%%%%%%%%%           %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%               \n");
    printf("%%%%%%%%%%                  %%%%%%%%%%%%%%%          %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%      %%%%  \n");
    printf("%%%%%%%%%@                   @%%%%%%%%%%%%%%         %%%%%%%%%%%%@ %%%% %%%%%%%%%%%%%%%%%   %%%%%%%%\n");
    printf("%%%%%%%%%%                  %%%%%%%%%%%%%%%%%        %%%%%%%%%%%%%      %%%%%%%%%%%%%%%%%% %%%%%%%%%\n");
    printf("%%%%%%%%%@%%@                %%%%%%%%%%%%%%%%@       %%%%%%%%%%%%%%     %%%%%%%%%%%%%%%%%%%%%%%%  %%\n");
    printf(" %%%%%%%%%%                  % %%%%%%%%%%%%%%@        %%%%%%%%%%%%%%   %%%%%%%%%%%%%%%%%%%%%%%%%% %%\n");
    printf("  %%%%%%%%%%%%  @           %%%%%%%%%%%%%%%%%%        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  %%% \n");
    printf("   %%%%%%%%%%%%% %%  %  %@ %%%%%%%%%%%%%%%%%%          %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    %%% \n");
    printf("    %%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%%%%%%%           @%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%    %%%%%%% \n");
    printf("     %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%              %%%%%%%%%%%%%%%%%%%%%%%%%%%%        %%%   \n");
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

int build_cmd_list(char *cmd_line, command_list_t *clist) {
    int command_count = 1;
    char *temp = cmd_line;

    if (strlen(cmd_line) == 0) {
        return WARN_NO_CMDS;
    }

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
        return -1;
    }

    char *command = strtok(cmd_line_copy, PIPE_STRING);

    while (command != NULL) {
        command = trim_spaces(command);

        if (strlen(command) >= SH_CMD_MAX) {
            free(cmd_line_copy);
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }

        // Handle executable
        char *exe_start = command;
        char *exe_end = strchr(command, SPACE_CHAR);
        if (exe_end != NULL) {
            *exe_end = '\0';
        }

        if (strlen(exe_start) >= EXE_MAX) {
            free(cmd_line_copy);
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }

        strncpy(clist->commands[clist->num].exe, exe_start, EXE_MAX - 1);
        clist->commands[clist->num].exe[EXE_MAX - 1] = '\0';

        if (exe_end != NULL) {
            // Handle arguments
            char *args_start = exe_end + 1;
            args_start = trim_spaces(args_start);

            if (strlen(args_start) >= ARG_MAX) {
                free(cmd_line_copy);
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }

            strncpy(clist->commands[clist->num].args, args_start, ARG_MAX - 1);
            clist->commands[clist->num].args[ARG_MAX - 1] = '\0';
        } else {
            clist->commands[clist->num].args[0] = '\0';
        }

        clist->num++;
        command = strtok(NULL, PIPE_STRING);
    }

    free(cmd_line_copy);
    return OK;
}
