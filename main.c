#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>

char **get_input(char input[]) {
    char **command = malloc(8 * sizeof(char *));
    char *parsed;
    int index = 0;

    parsed = strtok(input, " ");
    while (parsed != NULL) {
        command[index] = parsed;
        index++;

        parsed = strtok(NULL, " ");
    }

    command[index] = NULL;
    return command;
}

int main() {
    pid_t child_pid;
    int stat_loc;

    while (true) {
        char **command;
        char input[8 * sizeof(char*)];
        
        // getting input
        printf("Shell > ");
        scanf("%[^\n]", input);

        // tokenizing commands
        command = get_input(input);

        // creating child process
        child_pid = fork();
        if (child_pid == 0) {
            // inside child process
            // executing the commands
            execvp(command[0], command);
            printf("Invalid Command\n");
        } 
        else {
            // inside parent process
            // waiting for child to execute
            waitpid(child_pid, &stat_loc, WUNTRACED);
        }

        // clearing the std buffer
        char temp;
        scanf("%c", &temp);

        free(command);
    }

    return 0;
}