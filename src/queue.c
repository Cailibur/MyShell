#include <stdlib.h>
#include <stdio.h>

typedef struct str_node
{
    str_node *next;
    char *content;
}str_node;


typedef struct str_queue
{
    str_node *head;
}str_queue;

void push(str_queue *q, char *cont){
    str_node *new_node = (str_node*)malloc(sizeof(str_node));
    new_node->content = cont;
    new_node->next = q->head;
    q->head = new_node;
}