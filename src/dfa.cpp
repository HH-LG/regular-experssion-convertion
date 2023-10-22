#include "nfa.h"
#include <set>
#include <algorithm>
#include <stack>
#include "dfa.h"

using namespace std;

dfa dfa_generated;

// 判断集合a是否包含集合b
bool contains(const dState s1, const dState s2) 
{
    return includes(s1.states.begin(), s1.states.end(), s2.states.begin(), s2.states.end());
}

dState e_closure(dState s)
{
    dState T;
    stack<state*> stackStates;
    unordered_map<state*, bool> map;
    for (state* s0: s.states)
    {
        stackStates.push(s0);
    }

    while(!stackStates.empty())
    {
        state* curState = stackStates.top();
        stackStates.pop();
        map.emplace(curState, true); 
        T.states.insert(curState);
        for (auto edge: curState->edges)
        {
            vector<state*> nextStates = edge.second;
            char m_ch = edge.first;
            for (auto nextState: nextStates)
                if (map.find(nextState) == map.end() && m_ch == epsilon)
                {
                    stackStates.push(nextState); 
                }
        }
    }
    
    return T;
}
dState move(dState s, char ch)
{
    dState T;
    stack<state*> stackStates;
    unordered_map<state*, bool> map;
    for (state* s0: s.states)
    {
        stackStates.push(s0);
    }

    while(!stackStates.empty())
    {
        state* curState = stackStates.top();
        stackStates.pop();
        for (auto edge: curState->edges)
        {
            vector<state*> nextStates = edge.second;
            char m_ch = edge.first;
            for (auto nextState: nextStates)
                if (map.find(nextState) == map.end() && m_ch == ch)
                {
                    stackStates.push(nextState); 
                    map.emplace(nextState, true); 
                    T.states.insert(nextState);
                }
        }
    }

    return T;
}


void buildDFA(struct expr e)
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
    
    dState s;
    s.states.insert(start);
    dState s0 = e_closure(s);
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
    stack<dState> stateStack;
    set<dState> dfaStates;
    map<dState, int> state_to_index;

    stateStack.push(s0);
    //对于dfa的计算
    dfaStates.insert(s0);
    state_to_index[s0] = dfa_state_cnt++;

    while(!stateStack.empty())
    {
        dState curDState = stateStack.top();
        // printf("curDState: %d\n", state_to_index[curDState]);
        stateStack.pop();
        for (char ch: charSet)
        {
            if (ch == epsilon)
                continue;
            dState nextDState = e_closure(move(curDState, ch));
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