#ifndef DFA_H
#define DFA_H
#include <unordered_map>
#include <set>
#include <vector>
#include <map>
#include "nfa.h"
#include <algorithm>

using namespace std;

struct dState
{
    set<state*> states;

    bool operator<(const dState &s) const // 重载<运算符,这里仅仅需要将不同集合区分开，默认字典序足矣
    {
        return states < s.states;
    }
};
struct dfa
{
   map<int, map<char, int>> graph;
   set<int> finish_state;
};

extern dfa dfa_generated;

// 打印dfa
void printDFA(dfa dfa_t);

// 构造dfa
void buildDFA(struct expr e);
#endif