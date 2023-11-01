#include "nfa.h"
#include <unordered_map>
#include <iostream>
#include <set>
#include <vector>
#include <locale>
using namespace std;

set<char> charSet;
int CurrentState = 0;
struct state* newState()
{
    state* s = new state();
    s->id = CurrentState++;
    return s;
}

struct nfa* newExprval(char ch)
{
    //添加到字符集
    charSet.insert(ch);

    //新建状态
    struct state* start = newState();
    struct state* end = newState();
    addEdge(start, ch, end);
    return newExprval(start,end);
}

struct nfa* newExprval(struct state* start,struct state* end)
{
    struct nfa* expr = (struct nfa*)malloc(sizeof(struct nfa));
    expr->start = start;
    expr->end = end;
    return expr;
}

void printState(struct state* s)
{
    setlocale(LC_ALL, "");  // 设置全局区域设置

    bool stateUsed[MAX_STATE_SIZE] = {false};
    struct state* stateStack[MAX_STATE_SIZE/2];
    int size = 0;

    stateStack[size++] = s;

    printf("digraph G {\n");
    while(size)
    {
        struct state* curState = stateStack[--size];
        stateUsed[curState->id] = true;

        for (auto edge: curState->edges)
        {
            char ch = edge.first;
            wchar_t wch;
            mbtowc(&wch, &ch, MB_CUR_MAX);
            if (ch == epsilon)
                wch = L'\u03b5';

            vector<state*> nextStates = edge.second;
            for (auto nextState: nextStates)
                printf("\t%d -> %d [label=\"%lc\"];\n", curState->id, nextState->id, wch);
        }

        for (auto edge: curState->edges)
        {
            vector<state*> nextStates = edge.second;
            for (auto nextState: nextStates) 
            {
                if (!stateUsed[nextState->id])
                {
                    stateStack[size++] = nextState; 
                }
            }
        }
    }
    printf("}\n");
}

void printNFA(struct nfa* expr)
{
    printState(expr->start);
}

struct nfa* connectExprval(struct nfa* expr1,struct nfa* expr2)
{
    if (expr2 == NULL)
        return expr1;
    struct state *interS = expr1->end;
    struct state *interE = expr2->start;

    interS->edges = interE->edges;
    return newExprval(expr1->start,expr2->end);
}

void addEdge(struct state *s, char ch, struct state *nextState)
{
    // 添加到字符集
    charSet.insert(ch);

    // 用map添加一条边
    s->edges[ch].push_back(nextState);
}

struct nfa* closureExprval(struct nfa* expr)
{
    struct state *start = expr->start;
    struct state *end = expr->end;

    struct state *new_start = newState();
    struct state *new_end = newState();

    addEdge(end, epsilon, start); //反向边
    addEdge(new_start, epsilon, start);
    addEdge(end, epsilon, new_end);
    addEdge(new_start, epsilon, new_end);
    return newExprval(new_start, new_end);
}

struct nfa* orExprval(struct nfa* expr1,struct nfa* expr2)
{
    struct state *start1 = expr1->start;
    struct state *end1 = expr1->end;
    struct state *start2 = expr2->start;
    struct state *end2 = expr2->end;

    struct state *new_start = newState();
    struct state *new_end = newState();

    addEdge(new_start, epsilon, start1);
    addEdge(new_start, epsilon, start2);
    addEdge(end1, epsilon, new_end);
    addEdge(end2, epsilon, new_end);

    return newExprval(new_start, new_end);
}