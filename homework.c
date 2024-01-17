#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#define MAXLINE 80 /* The maximum length command */

int main(void)
{
    char *args[MAXLINE/2 + 1]; /* command line arguments */
    char lastCommand[MAXLINE]="";
    int should_run = 1; /* flag to determine when to exit program */
    while (should_run)
    {
        printf("21522625>");
        fflush(stdout);

        // Get input from user
        char buffer[MAXLINE];
        fgets(buffer, MAXLINE, stdin);
        
        //Modify input string  
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        if (buffer[0] == '\0') {
            continue;  // Handle none command
        }else if(strcmp(buffer, "exit") == 0)
        {
            break; // handle exit command 
        } else if (strcmp(buffer, "!!") == 0) { // Handle execute last command
            if(strcmp(lastCommand, "") == 0){
                printf("No commands in history.\n");
                continue;
            } else{
               strcpy(buffer, lastCommand);
                printf("21522625>%s \n", buffer);

            }
            
        }else {
            strcpy(lastCommand, buffer);
        }



        // parsing the command  
        int n = 0;
        char *token = strtok(buffer, " \n");
        while (token != NULL && n < MAXLINE/2 + 1) {
            args[n++] = token;
            token = strtok(NULL, " \n");
        }
        args[n] = NULL;


        // check if it run concurrently or not 
        bool waitFlag = true;
        if(*args[n-1] == '&'){
            waitFlag = false;
            args[n-1] = NULL; 
        }

        // Create fork for executing the command
        pid_t pid = fork();
        if(pid < 0) {
            printf("Have problem in creating child process\n");
            exit(1);
        }else if(pid == 0) {
            // child process executing the command 
            execvp(args[0], args);
            // If execvp fails, print an error message
            printf("%s: command not found\n",buffer);
            exit(1);
            
        }else {
            // Wait for the child process if it does not have &
            if(waitFlag){
                wait(NULL);
            }else {
                printf("%d\n",pid);
            }

        }
    }
    return 0;
}