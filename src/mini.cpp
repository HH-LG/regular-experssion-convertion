#include "dfa.h"
#include "nfa.h"
#include <set>
#include <map>

using namespace std;

dfa dfa_simplified;

void simplifyDFA()
{
    map<int, int> state_to_set_map;
    vector<set<int>> div;
    set<int> normal_state;
    for (auto state: dfa_generated.graph)
    {
        if (dfa_generated.finish_state.find(state.first) == dfa_generated.finish_state.end())
        {
            state_to_set_map[state.first] = 0;
            normal_state.insert(state.first);
        }
        else
        {
            state_to_set_map[state.first] = 1;
        }
    }
    div.push_back(normal_state);
    div.push_back(dfa_generated.finish_state);

    // int cnt = 0;
    while(true)
    {
        size_t now_size = div.size();
        for (char ch: charSet)
        {
            printf("current divide:\n");
            for (int i = 0;i < (int)div.size();i++)
            {
                printf("set:%d\n", i);
                for (int state: div[i])
                    printf("%d\t", state);
                printf("\n");
            }
            if (ch == epsilon)
                continue;
            map<int, int> new_state_to_set_map;
            map<int, int> set_to_new_set_map;
            vector<set<int>> new_div;
            for (set<int> part: div)
            {
                set<int> next_sets;
                for (int state: part)
                {
                    int next_state = dfa_generated.graph[state].find(ch) == dfa_generated.graph[state].end() ? 
                                    -1 : dfa_generated.graph[state][ch];
                    int next_set = state_to_set_map.find(next_state) == state_to_set_map.end() ? 
                                    -1 : state_to_set_map[next_state];
                    printf("state:%d -> next_state:%d, next_set:%d, ch:%c\n", state, next_state, next_set, ch);
                    if (next_sets.find(next_set) == next_sets.end())
                    {
                        printf("new set:%ld\n", new_div.size());
                        next_sets.insert(next_set);
                        set_to_new_set_map[next_set] = new_div.size();  // 旧集合到新集合的映射,对应到新的div的新划分中
                        new_div.push_back(set<int>());  //加入一个空的新集合
                    }
                    new_state_to_set_map[state] = set_to_new_set_map[next_set];
                    new_div[set_to_new_set_map[next_set]].insert(state);
                }
            }
            state_to_set_map = new_state_to_set_map; // 更新分划
            div = new_div;  // 更新集合
        }
        if (now_size == div.size())  // 一轮还未更新，退出
            break;
        //cnt++;
        //if (cnt == 10)
        //    break;
    }

    // 生成简化的dfa
    // 生成边 
    for (set<int> s: div)
    {
        for (char ch: charSet)
        {
            if (ch == epsilon)
                continue;
            int next_state = dfa_generated.graph[*s.begin()].find(ch) == dfa_generated.graph[*s.begin()].end() ? 
                            -1 : dfa_generated.graph[*s.begin()][ch];
            if (next_state == -1)
                continue;
            dfa_simplified.graph[state_to_set_map[*s.begin()]][ch] = state_to_set_map[next_state];
        }
    }

    // 生成终态
    for (int s: dfa_generated.finish_state)
    {
        dfa_simplified.finish_state.insert(state_to_set_map[s]);
    }
}