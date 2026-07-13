#ifndef PROMPT_H
#define PROMPT_H

#define PATH_MAX 4096
#define MAX_ARGS 64
#define MAX_LINE 1024

extern char cwd[PATH_MAX];
extern char line[MAX_LINE];
extern char *argv[MAX_ARGS];
extern int argc;
extern char *input_file;
extern char *old_pwd;

void parse_line(char* line , char* argv[]);
void handleMessage();

#endif

