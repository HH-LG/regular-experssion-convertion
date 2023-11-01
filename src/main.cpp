#include "nfa.h"
#include "dfa.h"
#include "test.h"
#include <iostream>
#include <unistd.h>
#include <string.h>

extern FILE *yyin;
extern int regularPrase();

using namespace std;

char testfile[256] = "./test/regex.txt";
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

int ot_flag = 0;

int main(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "to:")) != -1)
    {
        switch (opt)
        {
        case 't':
            if(ot_flag == 0)
            {
                ot_flag = 1;
            }
            else
            {
                fprintf(stderr, "Usage: %s -o outfile, %s -t\n", argv[0], argv[0]);
                exit(EXIT_FAILURE);
            }
            break;
        case 'o':
            if (ot_flag == 0)
            {
                strcpy(outfile, optarg);
                ot_flag = 2;
            }
            else
            {
                fprintf(stderr, "Usage: %s -o outfile, %s -t\n", argv[0], argv[0]);
                exit(EXIT_FAILURE);
            }
            break;
        default:
            fprintf(stderr, "Usage: %s -o outfile, %s -t\n", argv[0], argv[0]);
            exit(EXIT_FAILURE);
            break;
        }
    }

    // 测试一下 argc 与 argv
    //cout << argc <<endl;
    //for (int i=0; i < argc; i++)
    //{
    //    cout << argv[i] << endl;
    //}
    cout << "here" << endl;
    if (ot_flag == 1)
    {
        test_dfa_simplified(testfile);
    }
    else if (ot_flag == 2)
    {
        yyin = stdin;
        FILE *fp = freopen(outfile, "w", stdout);
        if (fp == NULL)
        {
            fprintf(stderr, "%s: fail to open output file\n", outfile);
            exit(EXIT_FAILURE);
        }
        regularPrase();

    }

}