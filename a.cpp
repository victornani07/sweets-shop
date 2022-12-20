#include <iostream>
#include <string>
#include <string.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <set>
#include <unordered_set>

using namespace std;

int main(void) {
    unordered_set<int> s;
    s.insert(2);
    s.insert(3);
    s.insert(1);
    s.insert(1);
    s.insert(2);
    s.insert(0);
    s.insert(0);
    s.insert(2);

    for (auto& str: s) {
        cout << str << ' ';
    }
}