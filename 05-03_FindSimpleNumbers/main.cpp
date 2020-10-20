#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <stdlib.h>
#include <ctime>
#include <cstring>

using namespace std;

bool IsSimpleNumber(int Number) {
    if(Number == 2 || Number == 3)
        return true;
    
    if(Number < 2 || Number % 2 == 0)
        return false;
    
    for(int i = 2; i <= sqrt(Number); i++)
        if(Number % i == 0)
            return false;
    
    return true;
}

vector<int> FindAllSimpleNumbers(const vector<int> &Numbers) {
    srand(time(NULL));
    vector<int> Result;
    
    for(int Number : Numbers) {
        if(IsSimpleNumber(Number))
            Result.push_back(Number);
    }
    
    return Result;
}


int main() {
    vector<int> Numbers;
    
    for(int i = 0; i < 20000; i++)
        Numbers.push_back(i);
    
    vector<int> Result = FindAllSimpleNumbers(Numbers);
    
    for(int Number : Result)
        cout << Number << " ";
    
    return 0;
}
