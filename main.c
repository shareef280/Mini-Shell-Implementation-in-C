
/*
 * Project : Mini Shell
 * Name    : Shareef sab
 * Batch   : 26001b_084
 * Date    : 29/08/2026
 * Description : A simple Linux shell that supports builtin commands,
 *                external commands, pipes and basic job control.
 */

#include "main_h.h"

char prompt[50] = "minishelll$";
char input_string[100];

int main()
{
    // Clear terminal
    system("clear");

    // Allocate memory for external commands
    external_commands = malloc(sizeof(char *) * 200);

    if (external_commands == NULL)
    {
        perror("malloc");
        return 1;
    }

    // Load external commands from file
    extract_external_commands();

    // Start minishell
    scan_input(prompt, input_string);

    // Free allocated memory
    free_memory(external_commands);

    return 0;
}

