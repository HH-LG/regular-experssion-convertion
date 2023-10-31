#include <iostream>
#include <fstream>
#include "test.h"

extern int regularPrase();
extern int yyparse();
extern FILE *yyin;

using namespace std;

void test_dfa_simplified(char* testfile)
{
    yyin = fopen(testfile, "r");
    if (yyin == NULL)
    {
        fprintf(stderr, "%s: fail to open input file\n", testfile);
        exit(EXIT_FAILURE);
    }       
    FILE *fp = freopen("./log/test.dot", "w", stdout);
    if (fp == NULL)
    {
        fprintf(stderr, "fail to open output file\n");
        exit(EXIT_FAILURE);
    }
    int cnt = 0;
    do
    {
        yyparse();

        cnt ++;
        cout << "cnt:" << cnt << endl;
    } while (!feof(yyin));
    
}

bool test_regex(int id)
{

}