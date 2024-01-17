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
    int should_run = 1; /* flag to determine when to exit program */
    while (should_run)
    {
        printf("21522625>");
        fflush(stdout);

        // Get input from user
        char buffer[MAXLINE];
        fgets(buffer, MAXLINE, stdin);
        
        // parsing the command  
        int n = 0;
        char *token = strtok(buffer, " \n");
        while (token != NULL && n < MAXLINE/2 + 1) {
            args[n++] = token;
            token = strtok(NULL, " \n");
        }
        args[n] = NULL;


        //check null command      
        if ( n==0) {continue;}

        // handle exit command 
        if (strcmp(args[0], "exit") == 0) {
            should_run =0;
            continue;  
        }

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
            perror("execvp");
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