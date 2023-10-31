#include <iostream>
#include <fstream>
#include "test.h"


using namespace std;

void test_dfa_simplified(char* testfile)
{
    ifstream regex_file(testfile);

    char buffer[256];
    cout << "inFile.txt" << "--- all file is as follows:---" << endl;
    while (!regex_file.eof())
    {
        regex_file.getline(buffer, 256, '\n'); //getline(char *,int,char) 表示该行字符达到256个或遇到换行就结束
        cout << buffer << endl;
    }

    regex_file.close();
}