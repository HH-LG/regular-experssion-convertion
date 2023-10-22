#include <locale.h>
#include <wchar.h>
#include <stdio.h>
#include <map>
#include <iostream>
using namespace std;
int main() {
    map<int, int> m;
    m[1] = 2;
    cout << (m.find(0) == m.end()) << endl; 
    return 0;
}
