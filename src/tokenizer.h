#ifndef TOKENIZER
#define TOKENIZER

#define MAX_TOK 2048

typedef enum {
    TOK_WORD,
    TOK_IN,
    TOK_OUT,
    TOK_APPEND,
    TOK_PIPE,
    TOK_AMP
} TokenType;

typedef struct Token {
    TokenType type;
    char *text;
} Token;

extern Token tokens[MAX_TOK];
extern int tok_cnt;

int tokenize();

#endif