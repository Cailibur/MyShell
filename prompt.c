#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<utime.h>
#include<time.h>
#include<pwd.h>
#include<limits.h>
#include<dirent.h>
#include<string.h>
#include"prompt.h"
#include"builtin.h"
#include"color.h"

char cwd[PATH_MAX];
char line[MAX_LINE];
char *argv[MAX_ARGS];
char *input_file;
int argc = 0;
char *old_pwd = NULL;

//Using tokenizer to split the input into argvs.
void parse_line(char* line , char* argv[]){
    argc = 0;
    char *token = strtok(line, " \t\n");
    while (token != NULL && argc < MAX_ARGS - 1) {
        if(strcmp(token, "<") == 0) {
            // Handle input redirection
            token = strtok(NULL, " \t\n");
            if (token != NULL) {
                input_file = token;
            } 
            else {
                fprintf(stderr, "Error: No input file specified for redirection.\n");
                return;
            }
        } 
        else {
            argv[argc++] = token;
        }
        token = strtok(NULL, " \t\n");
    }
    //printf("%d\n", argc);
    argv[argc] = NULL;
}

void handleMessage(){
    if(fgets(line , sizeof(line) , stdin) == NULL){
        return;
    }
    parse_line(line , argv);
    if(argc == 0){
        return;
    }
    else if(strcmp(argv[0], "pwd") == 0){
        pwd();
    }
    else if(strcmp(argv[0], "ls") == 0){
        ls();
    }
    else if(strcmp(argv[0], "echo") == 0){
        echo();
    }
    else if(strcmp(argv[0], "cd") == 0){
        cd();
    }
    else if(strcmp(argv[0], "mkdir") == 0){
        makedir();
    }
    else if(strcmp(argv[0], "touch") == 0){
        touch();
    }
    else if(strcmp(argv[0], "export") == 0){
        export();
    }
    else if(strcmp(argv[0], "unset") == 0){
        unset();
    }
    else if(strcmp(argv[0], "rm") == 0){
        rm();
    }
    else if(strcmp(argv[0], "help") == 0){
        printf(STYLE_BOLD COLOR_YELLOW"MyShell Help:\n");
        printf(STYLE_RESET COLOR_YELLOW"Built-in commands:\n");
        printf("pwd: Print the current working directory.\n");
        printf("ls: List files and directories in the current directory.\n");
        printf("echo: Display a line of text.\n");
        printf("cd: Change the current working directory.\n");
        printf("mkdir: Create a new directory.\n");
        printf("touch: Create a new empty file or update the access and modification times of an existing file.\n");
        printf("export: Set environment variables.\n");
        printf("unset: Unset environment variables.\n");
        printf("rm: Remove files or directories.\n");
        printf("exit: Exit the shell.\n");
    }
    else if(strcmp(argv[0], "exit") == 0){
        printf("Exiting MyShell...\n");
        exit(0);
    }
    else if(strcmp(argv[0], "clear") == 0){
        printf(CLEAR);
    }
    else{
        pid_t pid = fork();
        if(pid == 0){
            // Child process
            execvp(argv[0], argv);
            perror("execvp");
            exit(1);
        }
        else if(pid > 0){
            // Parent process
            int status;
            waitpid(pid, &status, 0);
        }
        else{
            perror("fork");
        }
    }
    //printf("%d\n" , argc);
    //printf("%s\n",input_file);
}