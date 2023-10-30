#include "nfa.h"
#include "dfa.h"
#include <iostream>
#include <unistd.h>
#include <string.h>

extern int regularPrase();

using namespace std;

char outfile[256] = "a.dot";

void test01()
{
    nfa* e1 = newExprval('a');
    nfa* e2 = newExprval('b');
    e1 = closureExprval(e1);
    e2 = closureExprval(e2);
    nfa* e3 = connectExprval(e1, e2);
    buildDFA(*e3);
    printDFA(dfa_generated);
    printNFA(e3);
}

int main(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "to:")) != -1)
    {
        switch (opt)
        {
        case 'o':
            strcpy(outfile, optarg);
            break;
        default:
            fprintf(stderr, "Usage: %s [-o outfile]\n", argv[0]);
            exit(EXIT_FAILURE);
            break;
        }
    }

    // 测试一下 argc 与 argv
    cout << argc <<endl;
    for (int i=0; i < argc; i++)
    {
        cout << argv[i] << endl;
    }

    FILE *fp = freopen(outfile, "w", stdout);  //重定向
    
    if (fp == NULL)
    {
        printf("error opening file\n");
        exit(-1);
    }
    
    regularPrase();
    fclose(fp);
}