#include <set>
#include <algorithm>
#include <iostream>
using namespace std;

int main()
{
    set<int> stats1;
    set<int> stats2;

    stats1.insert(1);
    stats2.clear();
    if (includes(stats1.begin(), stats1.end(), stats2.begin(), stats2.end()))
        cout << "yes" <<endl;
    else
        cout << "no" << endl;
}