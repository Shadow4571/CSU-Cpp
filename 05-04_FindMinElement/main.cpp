#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <stdlib.h>
#include <cstring>
#include <utility>

using namespace std;

double FindMinElement(const vector<double> &Elements1, const vector<double> &Elements2) {
    if(Elements1.empty() || Elements2.empty())
        throw "Vector cannot be empty";
    
    double Result = 0;
    bool Check = false;
    
    for(double Element : Elements1) {
        if(find(Elements2.begin(), Elements2.end(), Element) == Elements2.end()) {
            if(!Check) {
                Result = Element;
                Check = true;
            } else {
                if(Result > Element)
                    Result = Element;
            }
        }
    }
    
    if(!Check)
        throw "Element not found";
    
    return Result;
}

int main() {
    vector<double> Elements1 = vector<double>{2, 5, 6, 3, 5, 4, 8, 1};
    vector<double> Elements2 = vector<double>{5, 7, 6, 3, 6, 7, 0, 2};
    
    cout << "Result: " << FindMinElement(Elements1, Elements2);
    
    return 0;
}
