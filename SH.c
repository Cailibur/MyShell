#include<stdio.h>
#include<unistd.h>
#include<pwd.h>
#include<limits.h>
#include<dirent.h>
#include<string.h>
#include"prompt.h"
#include"color.h"


struct passwd *pw = NULL;
//PATH_MAX is defined in limits.h and specifies the maximum length of a path in the filesystem. 
//It is used to allocate a buffer that can hold the current working directory path.

void outputUser(){
    /*
    The struct passwd stores information about a user account.
    The fields of the struct passwd are as follows:
    *pw_name：用户名
    pw_uid：用户 ID
    pw_gid：主组 ID
    *pw_dir：家目录
    *pw_shell：默认登录 shell
    */
    if(pw == NULL) pw = getpwuid(getuid());
    if(getcwd(cwd, sizeof(cwd)) == NULL){
        perror("getcwd");
        return;
    }
    if(pw != NULL){
        printf(STYLE_BOLD COLOR_GREEN"[%s@MyShell]" COLOR_BLUE "%s" COLOR_RESET "$ ", pw->pw_name, cwd);
    }
    else{
        printf("Failed to get Username!\n");  
        return; 
    }
}

void outputPath(){
}

int main(){

    while(1){
        outputUser();
        outputPath();
        handleMessage();
    }

    return 0;
}