#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

int MaxInnerBrackets(string Expression) {
    vector<char> BRACKETS_OPEN = vector<char>({'(', '{', '[', '<'});
    vector<char> BRACKETS_CLOSE = vector<char>({')', '}', ']', '>'});

    int Result = 0;
    int Current = 0;

    for(int i = 0; i < Expression.size(); i++) {
        if(find(BRACKETS_OPEN.begin(), BRACKETS_OPEN.end(), Expression[i]) != BRACKETS_OPEN.end())
            Current++;

        if(find(BRACKETS_CLOSE.begin(), BRACKETS_CLOSE.end(), Expression[i]) != BRACKETS_CLOSE.end())
            Current--;

        if(Current > Result)
            Result = Current;
    }
    
    return Result;
}

int main() {
    string Expression = string("({([])<>})(){()(<>{()})}");

    cout << "Result: " << MaxInnerBrackets(Expression);

    return 0;
}