#include <string.h>

char *rm_n(char* str){
    int len = strlen(str);
    if(len > 0 && str[len-1] == '\n') str[len-1] = '\0';
    return str;
}