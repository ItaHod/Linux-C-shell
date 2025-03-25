#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <bits/waitflags.h>
#include <sys/wait.h>

#define LINE_LEN 1024
#define MAX_PARAMETERS 64


//process input - finds the comand + parameters
// argc is number of arguments
//args[0] is the command, other args are parameters
int process_input(int argc, char *args[MAX_PARAMETERS], char *input)
{
    char *token = strtok(input," ");
    while (token != NULL && argc < MAX_PARAMETERS)
    {
        args[argc] = token;
        argc++;
        token = strtok(NULL, " ");
     }
     args[argc] = NULL;
     return(argc);
}

int main()
{
    //we'll declare a background process structure
    struct bg_process {
    pid_t pid;
    char command[LINE_LEN];
    };
    
    struct bg_process bg_processes[4];
    int bg_count = 0;

    char input[LINE_LEN];

    while(1) //inf loop
    {
        char *args[MAX_PARAMETERS];
        int argc = 0;

        printf("hw1shell$ ");
        fflush(stdout);

       // Use fgets to read the input from the user
        if (fgets(input, sizeof(input), stdin) != NULL) 
        {
            input[strcspn(input, "\n")] = '\0'; //remove "tail" from end of input string
            if(strlen(input)==0) //check if the input was Enter/'\n' - if yes then print prompt again
            {
                continue;
            }
        } 
        else 
        {
            printf("invalid command\n");
        }

        argc = process_input(argc, args, input);
        
        //execute command

        //check if process is background
        int is_background = (strcmp(args[argc-1], "&") == 0);
        if(is_background)
        {
            args[argc-1] = NULL;
            argc--;
            if(bg_count >= 4 )
            {
                printf("hw1shell: too many background commands running\n");
                continue;
            } 
            else
            {
                bg_processes[bg_count].pid = fork();
                if (bg_processes[bg_count].pid < 0) 
                {
                    perror("fork");
                    continue;
                }
                strcpy(bg_processes[++bg_count].command , input);
            }
        }
        //internal commands
        if(strcmp(args[0], "exit") == 0)
        {
            //wait for background processes
            for (int i = 0; i < 4; i++)
            {
                if (bg_processes[i].pid != 0)
                {
                    waitpid(bg_processes[i].pid, NULL,0);
                }
            }
            break;
        }
        else if(strcmp(args[0],"cd") == 0)
        {
            if(argc <= 1)
            {
                printf("hw1shell: invalid command\n");
            }
            else
            {
                if (chdir(args[1]) != 0)
                {
                    printf("hw1shell: chdir failed, erno is %d\n",errno);
                }
            }
            continue;
        }
        else if(strcmp(args[0], "jobs") == 0)
        {
            for (int i = 0; i < 4; i++)
            {
                if(bg_processes[i].pid != 0)
                {
                    printf("%d\t%s\n",bg_processes[i].pid,bg_processes[i].command);
                }
            }
            continue;
        }

        //external commands
        pid_t pid = fork();
        if(pid < 0)     //fork error
        {            
            printf("hw1shell: fork failed, errno is: %d\n", errno);
            continue;
        }
        if (pid == 0)   //child process
        {          
            execvp(args[0], args);
            printf("hw1shell: invalid command\n");      //error executing command
            continue;
        } 
        else            //parent process
        {                 
            if(!is_background)      //foreground parent
            { 
                int status;
                if (waitpid(pid,&status,0) < 0)
                {
                    printf("hw1shell: waitpid failed, errno is %d\n", errno);
                }
            } 
            else
            {             //background parent
                for(int i = 0; i < 4; i++)
                {
                    if (bg_processes[i].pid == 0)
                    {
                        bg_processes[i].pid  = pid;
                        strcpy(bg_processes[i].command, input);
                        printf(" hw1shell: pid %d started\n", pid);
                        bg_count++;
                        break;
                    }
                }
            }
        }

        //reap background processes
        for(int i = 0; i < 4; i++)
        {
            pid_t w_no_hang = waitpid(bg_processes[i].pid,NULL,WNOHANG);
            if(w_no_hang > 0)
            {
                printf("hw1shell: pid %d finished \n", bg_processes[i].pid);
                bg_processes[i].pid = 0;
                bg_count--;
            }
        }

    }
    
}