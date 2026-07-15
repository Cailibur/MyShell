#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <utime.h>
#include <time.h>
#include <pwd.h>
#include <limits.h>
#include <dirent.h>
#include <string.h>
#include "prompt.h"
#include "builtin.h"
#include "color.h"
#include "tokenizer.h"

char cwd[PATH_MAX];
char line[MAX_LINE];
char tmp_line[MAX_LINE];
char *argv[MAX_ARGS];
char *input_file = NULL;
char *output_file = NULL;
int argc = 0;
char *old_pwd = NULL;
int bg_exe = 0;
int append = 0;

//Using tokenizer to split the input into argvs.
void parse_line(char* argv[]){
    argc = 0;
    append = 0;
    input_file = NULL;
    output_file = NULL;
    int p = 0;
    while (p < tok_cnt && argc < MAX_ARGS - 1) {
        if(tokens[p].type == TOK_IN) {
            // Handle input redirection
            p++;
            if (p >= tok_cnt) {
                fprintf(stderr, "Error: No input file specified for redirection.\n");
                return;
            } 
            else if(input_file != NULL){
                fprintf(stderr, "Error: multiple input redirections.\n");
                return;
            }
            input_file = tokens[p].text;
        } 
        else if(tokens[p].type == TOK_OUT){
            p++;
            if(p >= tok_cnt){
                fprintf(stderr, "Error: No input file specified for redirection.\n");
                return;
            }
            else if(output_file != NULL){
                fprintf(stderr, "Error: multiple output redirections.\n");
                return;
            }
            output_file = tokens[p].text;
        }
        else if(tokens[p].type == TOK_APPEND){
            p++;
            if(p >= tok_cnt){
                fprintf(stderr, "Error: No input file specified for redirection.\n");
                return;
            }
            else if(output_file != NULL){
                fprintf(stderr, "Error: multiple output redirections.\n");
                return;
            }
            output_file = tokens[p].text;
            append = 1;
        }
        else {
            argv[argc++] = tokens[p].text;
        }
        p++;
    }
    //printf("%d\n", argc);
    argv[argc] = NULL;
    if(argc > 0 && tokens[tok_cnt-1].type == TOK_AMP){
        argv[--argc] = NULL;
        bg_exe = 1;
    }
    else bg_exe = 0;
}

void write_history(const char *command) {
    FILE *history_file = fopen(".myshell_history", "a");
    if (history_file == NULL) {
        perror("fopen");
        return;
    }
    struct tm *cur_time;
    time_t raw_time;
    time(&raw_time);
    cur_time = localtime(&raw_time);
    char buffer[80];
    strftime(buffer, 80, "%Y年%m月%d日 %H:%M:%S", cur_time);
    fprintf(history_file, "%s %s", buffer, command);
    fclose(history_file);
}

void toknize(){
    if(fgets(line , sizeof(line) , stdin) == NULL) return;
    strcpy(tmp_line, line);
    write_history(line);
    if(tokenize() < 0){
        fprintf(stderr, "tokenizer: unexpected error.\n");
        return;
    }
    parse_line(argv);
}

void handleMessage(){
    //printf("%s %d\n" , argv[0], argc);
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
    else if(strcmp(argv[0], "history") == 0){
        history();
    }
    else if(strcmp(argv[0], "exit") == 0){
        printf("Exiting MyShell...\n");
        exit(0);
    }
    else if(strcmp(argv[0], "clear") == 0){
        clear();
    }
    else if(strcmp(argv[0], "sleep") == 0){
        builtin_sleep();
    }
    else if(strcmp(argv[0], "jobs") == 0){
        builtin_jobs();
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
    //printf("%s %s\n",input_file, output_file);
}