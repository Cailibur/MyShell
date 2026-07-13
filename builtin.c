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
#include"color.h"
#include "builtin.h"

void pwd(){
    //getcwd(char* buf, size_t size) is a function that retrieves the current working directory and stores it in the buffer pointed to by buf. 
    //The size parameter specifies the size of the buffer. 
    //If the current working directory is successfully retrieved, getcwd() returns a pointer to the buffer; 
    //otherwise, it returns NULL and sets errno to indicate the error.
    if(getcwd(cwd, sizeof(cwd)) != NULL){
        printf("Current Working Directory : %s\n" , cwd);
    }
    else{
        perror("getcwd");
    }
}

void ls(){
    //DIR is a dictionary Stream, while dirent is the current file you read on current dictionary stream.
    DIR *dir;
    if(opendir(argv[argc - 1]) != NULL){
        dir = opendir(argv[argc - 1]);
    }
    else{
        dir = opendir(".");
    }
    if(dir == NULL){
        perror("opendir");
    }
    struct dirent *entry;
    //readdir(DIR*) is like an iteraor that reads the next entry in the directory stream.
    //It returns a pointer to a struct dirent representing the next directory entry, or NULL if there are no more entries or an error occurs.
    while((entry = readdir(dir)) != NULL){
        printf("%s\n", entry->d_name);
    }
}

void echo(){
    for(int i = 1; i < argc; i++){
        //output all arguments after echo.
        printf("%s ", argv[i]);
    }
    printf("\n");
}

void cd(){
    if(argc < 2){
        //default
        old_pwd = getcwd(cwd, sizeof(cwd));
        chdir(getenv("HOME"));
    }
    else if(argc > 2){
        fprintf(stderr, "cd: too many arguments\n");
    }
    else{
        //chdir is used to change the current working dictionary.
        if(strcmp(argv[1], "~") == 0){
            chdir(getenv("HOME"));
        }
        else if(strcmp(argv[1], "-") == 0){
            if(old_pwd != NULL){
                chdir(old_pwd);
            }
            else{
                fprintf(stderr, "cd: OLDPWD not set\n");
            }
        }
        else{
            old_pwd = getcwd(cwd, sizeof(cwd));
            if(chdir(argv[1]) != 0){
                perror("cd");
            }
        } 
    }
}

void makedir(){
    if(argc < 2){
        fprintf(stderr, "mkdir: missing operand\n");
        return;
    }
    else{
        for(int i = 1 ; i < argc; i++){
            //mkdir() is a system call that creates a new directory with the specified name and permissions.
            //0755 is the permission mode for the new directory, which means that the owner has read, write, and execute permissions, while others have read and execute permissions.
            if(mkdir(argv[i], 0755) != 0){
                perror("mkdir");
                return;
            }
        }
    }
}

void touch(){
    if(argc < 2){
        fprintf(stderr, "touch: missing file operand\n");
        return;
    }
    else{
        for(int i = 1; i < argc; i++){
            int fd = open(argv[i], O_WRONLY | O_CREAT, 0644);
            if(fd < 0){
                perror("touch");
                return;
            }
            close(fd);
            // Update the access and modification times of the file to the current time
            struct utimbuf new_times;
            new_times.actime = time(NULL); // Set access time to current time
            new_times.modtime = time(NULL); // Set modification time to current time
            if(utime(argv[i], &new_times) != 0){
                perror("utime");
                return;
            }
        }
    }
}

void rm(){
    if(argc < 2){
        fprintf(stderr, "rm: missing operand\n");
        return;
    }

    for(int i = 1; i < argc; i++){
        if(unlink(argv[i]) != 0){
            perror(argv[i]);
            return;
        }
    }
}