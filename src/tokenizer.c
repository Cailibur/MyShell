#include <string.h>
#include <stdio.h>
#include "tokenizer.h"
#include "prompt.h"

Token tokens[MAX_TOK];
char *cur;
int tok_cnt = 0;
int l = 0, r = 0;
int len;

void add_tok(TokenType type){
    tokens[tok_cnt].type = type;
    tokens[tok_cnt].text = &line[l];
    tok_cnt++;
}

int tokenize(){
    tok_cnt = 0;
    l = r = 0;
    len = strlen(line);
    //printf("%s\n", line);
    while(line[r] != '\0'){
        //printf("%c %d\n", line[r], tok_cnt);
        if(line[r] == '\n' || line[r] == '\r'){
            if(l != r) add_tok(TOK_WORD);
            line[r] = '\0';
            return 0;
        }
        if(line[r] == ' '){
            line[r] = '\0';
            if(l != r) add_tok(TOK_WORD);
            r++;
            l = r;
            continue;
        }
        if(line[r] == '<'){
            if(l != r) add_tok(TOK_WORD);
            line[r] = '\0';
            l = r;
            r++;
            add_tok(TOK_IN);
            l = r;
            continue;
        }
        if(line[r] == '>'){
            if(line[r+1] == '>'){
                if(l != r) add_tok(TOK_WORD);
                line[r] = '\0';
                line[r+1] = '\0';
                l = r;
                r += 2;
                add_tok(TOK_APPEND);
                l = r;
                continue;
            }
            else{
                if(l != r) add_tok(TOK_WORD);
                line[r] = '\0';
                l = r;
                r++;
                add_tok(TOK_IN);
                l = r;
                continue;
            }
        }
        if(line[r] == '|'){
            if(l != r) add_tok(TOK_WORD);
            line[r] = '\0';
            l = r;
            r++;
            add_tok(TOK_PIPE);
            l = r;
            continue;
        }
        if(line[r] == '&'){
            if(l != r) add_tok(TOK_WORD);
            line[r] = '\0';
            l = r;
            r++;
            add_tok(TOK_AMP);
            l = r;
            continue;
        }
        if(line[r] == '"'){
            r++;
            while(r < len && line[r] != '"') r++;
            if(line[r] != '"'){
                fprintf(stderr, "tokenizer: unclosed double quote.\n");
                return -1;
            }
            r++;
            continue;
        }
        r++;
    }
    return 0;
}

