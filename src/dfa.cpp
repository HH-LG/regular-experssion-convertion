#include "nfa.h"
#include <set>
#include <algorithm>
#include <stack>
#include "dfa.h"

using namespace std;

dfa dfa_generated;

// 判断集合a是否包含集合b
bool contains(const state_set s1, const state_set s2) 
{
    return includes(s1.states.begin(), s1.states.end(), s2.states.begin(), s2.states.end());
}

state_set e_closure(state_set s)
{
    state_set T;
    stack<state*> stackStates;
    for (state* s0: s.states)
    {
        stackStates.push(s0);
    }

    while(!stackStates.empty())
    {
        state* curState = stackStates.top();
        stackStates.pop();
        T.states.insert(curState);

        if (curState->edges.find(epsilon) == curState->edges.end())
            continue;
        auto nextStates = curState->edges[epsilon];
        for (auto nextState: nextStates)
        {
            if (T.states.find(nextState) == T.states.end())
            {
                stackStates.push(nextState); 
            }
        }
    }
    
    return T;
}
state_set move(state_set s, char ch)
{
    state_set T;                        // 返回的状态集合
    stack<state*> stackStates;          // state的栈
    for (state* s0: s.states)   // 将输入中的每个状态入栈
    {
        stackStates.push(s0);
    }

    while(!stackStates.empty()) // 栈不空
    {
        state* curState = stackStates.top(); // 取出栈顶
        stackStates.pop();

        if (curState->edges.find(ch) == curState->edges.end()) // 没找到合适的边
            continue;
        auto nextStates = curState->edges[ch];
        for (auto nextState: nextStates)// 对该集合所有状态
        {
            if ( T.states.find(nextState) == T.states.end()) // 如果未找到该状态
            {
                // 加入状态集合中
                stackStates.push(nextState); 
                T.states.insert(nextState);
            }
        }
    }

    return T;
}


void buildDFA(struct nfa e)
{
    //清空已有的dfa
    dfa_generated.graph.clear();
    dfa_generated.finish_state.clear();
    // 死状态
    // bool have_dead = false;

    // 初始化dfa
    state* start = e.start;
    state* end = e.end;
    int dfa_state_cnt = 0;
    
    state_set s;
    s.states.insert(start);
    state_set s0 = e_closure(s);
    printf("e_closure(s0): ");
    for (state *s0: s0.states)
    {
        printf("%d ", s0->id);
    }
    printf("\n");
    if (s0.states.find(end) != s0.states.end())
    {   
        printf("finish state: %d\n", dfa_state_cnt);
        dfa_generated.finish_state.insert(dfa_state_cnt);
    }
    stack<state_set> stateStack;
    set<state_set> dfaStates;
    map<state_set, int> state_to_index;

    stateStack.push(s0);
    //对于dfa的计算
    dfaStates.insert(s0);
    state_to_index[s0] = dfa_state_cnt++;

    while(!stateStack.empty())
    {
        state_set curDState = stateStack.top();
        // printf("curDState: %d\n", state_to_index[curDState]);
        stateStack.pop();
        for (char ch: charSet)
        {
            if (ch == epsilon)
                continue;
            state_set nextDState = e_closure(move(curDState, ch));
            printf("e_closure(move(%d, %c)): ", state_to_index[curDState], ch);
            for (state *s0: nextDState.states)
            {
                printf("%d ", s0->id);
            }
            printf("\n");
            if (nextDState.states.empty())
            {
                continue;;
            }
            // 判断是否已经包含
            bool isContained = dfaStates.find(nextDState) != dfaStates.end();
            if (!isContained)
            {
                printf("state: %d\n", dfa_state_cnt);
                // 加入新的状态
                stateStack.push(nextDState);
                //对于dfa的计算
                dfaStates.insert(nextDState);
                if (nextDState.states.find(end) != nextDState.states.end())
                {   
                    // printf("finish state: %d\n", dfa_state_cnt);
                    dfa_generated.finish_state.insert(dfa_state_cnt);
                }
                state_to_index[nextDState] = dfa_state_cnt++;
            }
            dfa_generated.graph[state_to_index[curDState]][ch] = state_to_index[nextDState]; 
        }
    }
    
}

void printDFA(dfa dfa_t)
{
   // printf("charset: ");
   // for (char ch: charSet)
   // {
   //     printf("%c ", ch);
   // }
    printf("digraph G {\n");
    for (auto state: dfa_t.graph)
    {
        for (auto edge: state.second)
        {
            printf("\t%d -> %d [label=\"%c\"];\n", state.first, edge.second, edge.first);
        }
    }
    for (auto state: dfa_t.finish_state)
    {
        printf("\t%d [shape=doublecircle];\n", state);
    }
    printf("}\n");
}