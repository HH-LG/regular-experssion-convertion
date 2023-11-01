#include <iostream>
#include <fstream>
#include <unistd.h>
#include <iomanip>
#include <string>
#include <cstring>
#include "test.h"
#include "mini.h"

extern int regularPrase();
extern int yyparse();
extern FILE *yyin;

using namespace std;
bool test_regex(int id)
{
    int match_cnt = 0;
    int cur_state = 0;
    string filename = "./test/eg/example" + to_string(id) + ".txt";
    ifstream fin(filename);
    char input[256];
    while (!fin.eof())
    {
        fin.getline(input, 256, '\n'); //getline(char *,int,char) 表示该行字符达到256个或遇到换行就结束
        int len = strlen(input);
        for (int i = 0; i < len ; i++)
        {
            cur_state = dfa_simplified.graph[cur_state][input[i]];
        }
        if (dfa_simplified.finish_state.find(cur_state) != dfa_simplified.finish_state.end())
        {
            match_cnt ++;
        }
    }
    if (!fin.eof())
    {
        cout << "cur_state: " << cur_state << endl;
        cout << "test " << id << ", example " << match_cnt << " failed" << endl;
        fin.close();
        return false;
    }
    fin.close();
    return true;
}

void test_dfa_simplified(char* testfile)
{
    yyin = fopen(testfile, "r");
    if (yyin == NULL)
    {
        fprintf(stderr, "%s: fail to open input file\n", testfile);
        exit(EXIT_FAILURE);
    }       
    //FILE *fp = freopen("./log/test.dot", "w", stdout);
    //if (fp == NULL)
    //{
        //fprintf(stderr, "fail to open output file\n");
        //exit(EXIT_FAILURE);
    //}
    int cnt = 0;
    do
    {
        if(yyparse()) // 检测到q就跳出
            break;
        if (!test_regex(cnt))
        {
            cout << "test " << cnt << " failed" << endl;
            exit(EXIT_FAILURE);
        }
        cnt ++;
        cout << "\rTesting for generated dfa: " << std::fixed << std::setprecision(1)<< (float)cnt/Test_num*100.0 << "%" << flush;
    } while (!feof(yyin));
    cout << endl;
    usleep(500000); // 暂停500毫秒
    cout << "Testing complete, all tests passed!!!" << endl;
}

