#ifndef NFA_H
#define NFA_H
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <unordered_map>
#include <set>
#include <vector>
using namespace std;

#define epsilon '#'

// 将全局变量extern写在头文件，这样所有的文件都能使用
extern int CurrentState;
extern set<char> charSet;
// 状态的定义
#define MAX_NEXT_STATE 100
#define MAX_STATE_SIZE 100

struct state    // 状态
{
    int id;
    unordered_map<char, vector<state*>> edges;
};
struct nfa // NFA的结构
{
    struct state* start;
    struct state* end;
};


/* 操控状态的函数 */
// 
void addEdge(struct state *s, char ch, struct state *nextState);
// 创建新的状态
state* newState();
// 创建新的表达式
nfa* newExprval(char ch);

nfa* newExprvalSE(struct state* start,struct state* end);
// 连接两个表达式
nfa* connectExprval(struct nfa* expr1,struct nfa* expr2);
// 闭包
nfa* closureExprval(struct nfa* expr);
// 或
nfa* orExprval(struct nfa* expr1,struct nfa* expr2);
// 打印状态
void printState(struct state* s);
// 打印exprval
void printNFA(struct nfa* expr);

#endif