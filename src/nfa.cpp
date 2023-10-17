#include "nfa.h"

int CurrentState = 0;
struct state* newState(struct edge* nextEdge, int nextNum)
{
    struct state* s = (struct state*)malloc(sizeof(struct state));
    s->id = CurrentState++;
    s->nextNum = nextNum;
    if (nextNum > 0)
    {
        for (int i = 0; i < nextNum; i++)
        {
            s->nextEdge[i] = nextEdge[i];
        }
    }
    else
    {
        s->nextEdge[0].nextState = NULL;
    }
    return s;
}

struct expr* newExprval(char ch)
{
    struct state* start = newState(NULL,0);
    struct state* end = newState(NULL,0);
    addEdge(start, ch, end);
    return newExprvalSE(start,end);
}

struct expr* newExprvalSE(struct state* start,struct state* end)
{
    struct expr* expr = (struct expr*)malloc(sizeof(struct expr));
    expr->start = start;
    expr->end = end;
    return expr;
}

void printState(struct state* s)
{
    bool stateUsed[MAX_STATE_SIZE] = {false};
    struct state* stateStack[MAX_STATE_SIZE/2];
    int size = 0;

    stateStack[size++] = s;

    FILE *fp = freopen("output.dot", "w", stdout);  //重定向
    if (fp == NULL)
    {
        printf("error opening file\n");
        exit(-1);
    }

    printf("digraph G {\n");
    while(size)
    {
        struct state* curState = stateStack[--size];
        
        stateUsed[curState->id] = true;
        if (curState->nextNum == 0)
            continue;

        for (int i = 0; i < curState->nextNum; i++)
        {
            printf("\t%d -> %d [label=\"%c\"];\n", curState->id, curState->nextEdge[i].nextState->id, curState->nextEdge[i].ch);
        }

        for (int i = 0; i < curState->nextNum; i++)
        {
            if (!stateUsed[curState->nextEdge[i].nextState->id])
            {
                stateStack[size++] = curState->nextEdge[i].nextState;
            }
        }
    }
    printf("}\n");
    fclose(fp);
}

void printExprval(struct expr* expr)
{
    printState(expr->start);
}

struct expr* connectExprval(struct expr* expr1,struct expr* expr2)
{
    if (expr2 == NULL)
        return expr1;
    struct state *interS = expr1->end;
    struct state *interE = expr2->start;
    interS->nextNum = interE->nextNum;

    for (int i = 0; i< interE->nextNum; i++)
    {
        interS->nextEdge[i] = interE->nextEdge[i];
    }
    return newExprvalSE(expr1->start,expr2->end);
}

void addEdge(struct state *s, char ch, struct state *nextState)
{
    struct edge e;
    e.ch = ch;
    e.nextState = nextState;
    s->nextEdge[s->nextNum++] = e;
}

struct expr* closureExprval(struct expr* expr)
{
    struct state *start = expr->start;
    struct state *end = expr->end;

    struct state *new_start = newState(NULL, 0);
    struct state *new_end = newState(NULL, 0);

    addEdge(end, epsilon, start); //反向边
    addEdge(new_start, epsilon, start);
    addEdge(end, epsilon, new_end);
    addEdge(new_start, epsilon, new_end);
    return newExprvalSE(new_start, new_end);
}

struct expr* orExprval(struct expr* expr1,struct expr* expr2)
{
    struct state *start1 = expr1->start;
    struct state *end1 = expr1->end;
    struct state *start2 = expr2->start;
    struct state *end2 = expr2->end;

    struct state *new_start = newState(NULL, 0);
    struct state *new_end = newState(NULL, 0);

    addEdge(new_start, epsilon, start1);
    addEdge(new_start, epsilon, start2);
    addEdge(end1, epsilon, new_end);
    addEdge(end2, epsilon, new_end);

    return newExprvalSE(new_start, new_end);
}