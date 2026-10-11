#include "main_h.h"

char **external_commands;

char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs",
                    "let", "eval", "set", "unset", "export", "declare", "typeset",
                    "readonly", "getopts", "source", "exit", "exec", "shopt",
                    "caller", "true", "type", "hash", "bind", "help",
                    "fg", "bg", "jobs", "clear", NULL};

void scan_input(char *prompt, char *input_string)
{
    // Register signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGTSTP, signal_handler);
    //signal(SIGCHLD, signal_handler);

    while(1)
    {
        // Display prompt
        printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET " ", prompt);
        scanf(" %[^\n]", input_string);

        // Handle PS1 assignment
        if(strncmp(input_string, "PS1=", 4) == 0)
        {
            if(strchr(input_string, ' '))
            {
                printf(ANSI_COLOR_RED "PS1: invalid assignment\n"
                       ANSI_COLOR_RESET);
            }
            else
            {
                strcpy(prompt, input_string + 4);
            }
        }
        else
        {
            // Get command name
            char *command = get_command(input_string);

            // Check command type
            int ret = check_command_type(command);

            if(ret == BUILTIN)
            {
                execute_internal_commands(input_string);
            }
            else if(ret == EXTERNAL)
            {
                execute_external_commands(input_string);
            }
            else if(ret == NO_COMMAND)
            {
                printf(ANSI_COLOR_RED "%s: command not found\n"
                       ANSI_COLOR_RESET, command);
            }

            free(command);
        }
    }
}

char *get_command(char *input_string)
{
    // Extract first word from input
    char *ptr = malloc(strlen(input_string) + 1);
    int i = 0;
    int k = 0;

    while(input_string[i] != ' ' && input_string[i] != '\0')
    {
        ptr[k++] = input_string[i++];
    }

    ptr[k] = '\0';

    return ptr;
}

int check_command_type(char *command)
{
    // Check builtin commands
    for(int i = 0; builtins[i]; i++)
    {
        if(strcmp(builtins[i], command) == 0)
        {
            return BUILTIN;
        }
    }

    // Check external commands
    int i = 0;

    while(external_commands[i])
    {
        if(strcmp(command, external_commands[i]) == 0)
        {
            return EXTERNAL;
        }

        i++;
    }

    return NO_COMMAND;
}

void extract_external_commands()
{
    char ch = '\0';
    char command[50];
    int pos = 0;
    int index = 0;

    // Open external command file
    int fd = open("external_commd.txt", O_RDONLY);

    if(fd == -1)
    {
        printf("file not opened!!!\n");
        return;
    }

    // Read commands from file
    while(read(fd, &ch, 1) > 0)
    {
        if(ch == '\n')
        {
            // Remove carriage return
            if(pos > 0 && command[pos - 1] == '\r')
            {
                pos--;
            }

            command[pos] = '\0';

            if(pos > 0)
            {
                external_commands[index] = malloc(pos + 1);

                if(external_commands[index] == NULL)
                {
                    perror("malloc");
                    close(fd);
                    return;
                }

                strcpy(external_commands[index], command);
                index++;
            }

            pos = 0;
        }
        else
        {
            command[pos++] = ch;
        }
    }

    // Store last command
    if(pos > 0)
    {
        command[pos] = '\0';

        external_commands[index] = malloc(pos + 1);

        if(external_commands[index] == NULL)
        {
            perror("malloc");
            close(fd);
            return;
        }

        strcpy(external_commands[index], command);
        index++;
    }

    // Mark end of command list
    external_commands[index] = NULL;

    close(fd);
}