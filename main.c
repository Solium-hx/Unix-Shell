#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>
#include <signal.h>
#include <ctype.h>

#define DEFAULT "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[0;34m"


char **get_input(char input[]) {
    char **command = malloc(8 * sizeof(char *));
    if (command == NULL) {
        perror("malloc failed");
        exit(1);
    }

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

int cd (char* path) {
    return chdir(path);
}

int main() {
    pid_t child_pid;
    int stat_loc;
    bool running = true;

    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
        exit(1);
    }

    while (running) {
        char **command;
        char input[8 * sizeof(char*)];
        
        // getting input
        printf(YELLOW);
        printf("unixsHELL : #%s >> ", cwd);
        printf(DEFAULT);
        scanf("%[^\n]", input);
        
        // clearing the std buffer
        char temp;
        scanf("%c", &temp);

        // tokenizing commands
        command = get_input(input);

        if (strcmp(command[0], "cd") == 0) {
            if(cd(command[1]) < 0){
                perror(command[1]);
            }

            if (getcwd(cwd, sizeof(cwd)) == NULL) {
                perror("getcwd() error");
            }
        }
        else{
            // creating child process
            child_pid = fork();
            if (child_pid < 0) {
                perror("Fork Failed!");
            }

            if (child_pid == 0) {
                // inside child process
                
                // executing the commands
                printf(GREEN);
                if (execvp(command[0], command) < 0) {
                    perror(command[0]);
                }
            } 
            else {
                // inside parent process
                
                // waiting for child to execute
                waitpid(child_pid, &stat_loc, WUNTRACED);
            }
        }

        free(command);
    }


    return 0;
}