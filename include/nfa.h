#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<stdbool.h>

#define epsilon '#'

extern int CurrentState;
// 状态的定义
#define MAX_NEXT_STATE 100
#define MAX_STATE_SIZE 100
struct edge     // 边
{
    char ch;
    struct state* nextState;
};
struct state    // 状态
{
    int id;
    struct edge nextEdge[MAX_NEXT_STATE];
    int nextNum;
};
struct expr // 表达式的值
{
    struct state* start;
    struct state* end;
};

/* 操控状态的函数 */
// 
void addEdge(struct state *s, char ch, struct state *nextState);
// 创建新的状态
struct state* newState(struct edge* nextEdge, int nextNum);
// 创建新的表达式
struct expr* newExprval(char ch);
struct expr* newExprvalSE(struct state* start,struct state* end);
// 连接两个表达式
struct expr* connectExprval(struct expr* expr1,struct expr* expr2);
// 闭包
struct expr* closureExprval(struct expr* expr);
// 或
struct expr* orExprval(struct expr* expr1,struct expr* expr2);
// 打印状态
void printState(struct state* s);
// 打印exprval
void printExprval(struct expr* expr);