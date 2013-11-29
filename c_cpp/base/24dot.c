#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 1
#define MALLOC_COUNT(A) (malloc_count++, malloc(A))

static int malloc_count = 0;

static const char* test = "1+2+3*4";

typedef enum operator
{
    ADD,
    MINUS,
    MULTI,
    DIV
} Operator;

typedef struct frag
{
    int oper_left;
    int oper_right;
    Operator op;
} Frag;

typedef struct frag_list
{
    Frag data;
    struct frag_list * next;
} FragList;

static FragList * frag_list_head = NULL, * frag_list_tail = NULL;


void push_frag(Frag data)
{
    FragList * pFl = MALLOC_COUNT(sizeof(FragList));
    memcpy(&pFl->data, &data, sizeof(Frag));
    pFl->next = NULL;
    frag_list_tail->next = pFl;
    frag_list_tail = pFl;
}

FragList * pop()
{
    FragList * cur_header = frag_list_head;
    frag_list_head = frag_list_head->next;
    return cur_header;
}

void parse_frag(const char* num)
{
    int i = 0;
    int 
    for(i = 0; i < strlen(num); i++)

}

int calc_frag(Frag * frag)
{
    if(NULL == frag) return -1;
    switch(frag->op)
    {
        case ADD:
            return frag->oper_left + frag->oper_right;
        case MINUS:
            return frag->oper_left - frag->oper_right;
        case MULTI:
            return frag->oper_left * frag->oper_right;
        case DIV:
            if(frag->oper_left % frag->oper_right != 0)
                return -2;
            return frag->oper_left / frag->oper_right;
        default:
            break;
    }
    return -3;
}

int main()
{
    return 0;
}
