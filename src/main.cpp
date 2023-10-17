#include "nfa.h"
#include <iostream>
using namespace std;

void test01()
{
    struct state *s = newState(NULL, 0);
    addEdge(s, epsilon, s);
    printState(s);
}

int main(int argc, char *argv[])
{
    cout << argc <<endl;
    for (int i=0; i < argc; i++)
    {
        cout << argv[i] << endl;
    }
    // test01();
}