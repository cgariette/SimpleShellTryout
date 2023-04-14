#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024

void print_prompt() {
    printf("$ ");
}

void read_input(char* input) {
    fgets(input, MAX_COMMAND_LENGTH, stdin);
}

void parse_input(char* input, char** args) {
    char* token = strtok(input, " \t\n");
    int i = 0;
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[i] = NULL;
}

void execute_command(char** args) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        execvp(args[0], args);
        // If execvp returns, there was an error
        printf("Error executing command\n");
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking
        printf("Error forking process\n");
    } else {
        // Parent process
        waitpid(pid, NULL, 0);
    }
}

int main() {
    char input[MAX_COMMAND_LENGTH];
    char* args[MAX_COMMAND_LENGTH];
    while (1) {
        print_prompt();
        read_input(input);
        parse_input(input, args);
        if (args[0] != NULL) {
            if (strcmp(args[0], "exit") == 0) {
                // Exit shell program
                exit(EXIT_SUCCESS);
            }
            execute_command(args);
        }
    }
    return 0;
}
