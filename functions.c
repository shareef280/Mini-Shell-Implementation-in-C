
#include "main_h.h"

int status;
int pid = 0;
int ind;
Pro_list *head = NULL;

void execute_internal_commands(char *input_string)
{
    // Execute exit command
    if(strcmp(input_string, "exit") == 0)
    {
        exit(0);
    }
    // Execute pwd command
    else if(strcmp(input_string, "pwd") == 0)
    {
        char str[100];
        getcwd(str, 100);
        printf("%s\n", str);
    }
    // Change to home directory
    else if(strcmp(input_string, "cd") == 0)
    {
        chdir(getenv("HOME"));
    }
    // Change current directory
    else if(strncmp(input_string, "cd ", 3) == 0)
    {
        if(chdir(input_string + 3) == -1)
            perror("cd");
    }
    // Execute echo command
    else if(strncmp(input_string, "echo", 4) == 0 &&
            (input_string[4] == ' ' || input_string[4] == '\0'))
    {
        if(strstr(&input_string[5], "$SHELL"))
        {
            printf("%s\n", getenv("SHELL"));
        }
        else if(strstr(&input_string[5], "$?"))
        {
            if(WIFEXITED(status))
                printf("%d\n", WEXITSTATUS(status));
        }
        else if(strstr(input_string + 5, "$$"))
        {
            printf("%d\n", getpid());
        }
        else
        {
            printf("%s\n", input_string + 5);
        }
    }
    // Display stopped processes
    else if(strcmp(input_string, "jobs") == 0)
    {
        print_list();
    }
    // Bring latest process to foreground
    else if(strcmp(input_string, "fg") == 0)
    {
        if(head == NULL)
        {
            printf(ANSI_COLOR_RED "fg: no current job\n"
                   ANSI_COLOR_RESET);
            return;
        }

        kill(head->pid, SIGCONT);
        waitpid(head->pid, &status, WUNTRACED);
        pid = 0;
        delete_proccess();
    }
    // Continue latest process in background
    else if(strcmp(input_string, "bg") == 0)
    {
        if(head == NULL)
        {
            printf(ANSI_COLOR_RED "bg: no current job\n"
                   ANSI_COLOR_RESET);
            return;
        }

        kill(head->pid, SIGCONT);
        delete_proccess();
    }
    // Clear terminal
    else if(strcmp(input_string, "clear") == 0)
    {
        system("clear");
    }
    else
    {
        printf(ANSI_COLOR_RED "%s: command not found\n"
               ANSI_COLOR_RESET, input_string);
    }
}

void signal_handler(int sig_num)
{
    // Handle Ctrl+C
    if(sig_num == SIGINT)
    {
        if(pid == 0)
        {
            printf("\n");
            printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET " ", prompt);
            fflush(stdout);
        }
    }
    // Handle Ctrl+Z
    else if(sig_num == SIGTSTP)
    {
        if(pid == 0)
        {
            printf("\n");
            printf(ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET " ", prompt);
            fflush(stdout);
        }
        else
        {
            insert_proccess(pid, input_string);
        }
    }
    // Handle unknown signals
    else
    {
        printf("signal not registered\n");
    }
}

void execute_external_commands(char *input_string)
{
    char **commands = malloc(sizeof(char *) * 20);

    // Convert input into argument array
    create_2d_string(commands, input_string);

    // Check for pipe
    if(strchr(input_string, '|'))
    {
        execute_n_pipes(commands);
    }
    else
    {
        pid = fork();

        if(pid > 0)
        {
            // Parent waits for child
            waitpid(pid, &status, WUNTRACED);
            pid = 0;
        }
        else if(pid == 0)
        {
            // Restore default signals in child
            signal(SIGINT, SIG_DFL);
            signal(SIGTSTP, SIG_DFL);

            execvp(commands[0], commands);

            perror("execvp");
            exit(1);
        }
    }

    // Free command memory
    free_memory(commands);
}

void create_2d_string(char **commands, char *str)
{
    int i = 0;
    ind = 0;
    char cmd[50];
    int k = 0;

    // Split input into arguments
    while(str[i])
    {
        if(str[i] == ' ')
        {
            if(k > 0)
            {
                cmd[k] = '\0';

                commands[ind] = malloc(k + 1);
                strcpy(commands[ind], cmd);

                ind++;
            }

            k = 0;
        }
        else
        {
            cmd[k++] = str[i];
        }

        i++;
    }

    // Store last argument
    if(k > 0)
    {
        cmd[k] = '\0';

        commands[ind] = malloc(k + 1);
        strcpy(commands[ind], cmd);

        ind++;
    }

    // Mark end of arguments
    commands[ind] = NULL;
}

void execute_n_pipes(char **argv)
{
    int argc = ind;
    int i, j;
    int pipefd[2];
    int prev_read = -1;

    // Replace pipe symbols with NULL
    for(i = 0; i < argc; i++)
    {
        if(strcmp(argv[i], "|") == 0)
            argv[i] = NULL;
    }

    // Create processes for each command
    for(i = 0; i < argc; i++)
    {
        if(argv[i] == NULL)
            continue;

        // Find end of current command
        for(j = i; j < argc; j++)
        {
            if(argv[j] == NULL)
                break;
        }

        // Create pipe if another command exists
        if(j < argc - 1)
        {
            pipe(pipefd);
        }

        pid = fork();

        if(pid == 0)
        {
            // Restore default signals in child
            signal(SIGINT, SIG_DFL);
            signal(SIGTSTP, SIG_DFL);

            // Connect previous pipe to stdin
            if(prev_read != -1)
            {
                dup2(prev_read, STDIN_FILENO);
                close(prev_read);
            }

            // Connect output to next pipe
            if(j < argc - 1)
            {
                dup2(pipefd[1], STDOUT_FILENO);

                close(pipefd[0]);
                close(pipefd[1]);
            }

            // Execute command
            execvp(argv[i], &argv[i]);

            perror("execvp");
            exit(1);
        }

        // Close previous pipe in parent
        if(prev_read != -1)
            close(prev_read);

        // Save read end for next command
        if(j < argc - 1)
        {
            close(pipefd[1]);
            prev_read = pipefd[0];
        }

        // Move to next command
        i = j;
    }

    // Wait for all pipe processes
    while(wait(NULL) > 0)
    {
    }

    pid = 0;
}

void insert_proccess(int pid, char *str)
{
    Pro_list *new = malloc(sizeof(Pro_list));

    // Insert first process
    if(head == NULL)
    {
        new->pid = pid;
        strcpy(new->proccess, str);
        new->link = NULL;
        head = new;

        return;
    }

    // Insert process at beginning
    new->pid = pid;
    strcpy(new->proccess, str);
    new->link = head;
    head = new;
}

void delete_proccess()
{
    // Delete latest process
    if(head != NULL)
    {
        Pro_list *temp = head;
        head = head->link;
        free(temp);
    }
}

void print_list()
{
    Pro_list *temp = head;
    int i = 1;

    // Display stopped processes
    while(temp)
    {
        printf(ANSI_COLOR_YELLOW "[%d]" ANSI_COLOR_RESET
               " %d    %s\n",
               i,
               temp->pid,
               temp->proccess);

        temp = temp->link;
        i++;
    }
}

void free_memory(char **str)
{
    // Free argument strings
    for(int i = 0; str[i]; i++)
    {
        free(str[i]);
    }

    // Free argument array
    free(str);
}
