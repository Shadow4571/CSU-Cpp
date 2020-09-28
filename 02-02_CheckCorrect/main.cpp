#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

bool CheckBracketExpression(string Expression) {
    vector<char> BRACKETS_OPEN = vector<char>({'(', '{', '[', '<'});
    vector<char> BRACKETS_CLOSE = vector<char>({')', '}', ']', '>'});

    stack<char> LastOpen = stack<char>();

    for(int i = 0; i < Expression.size(); i++) {
        if(find(BRACKETS_OPEN.begin(), BRACKETS_OPEN.end(), Expression[i]) != BRACKETS_OPEN.end()) {
            LastOpen.push(Expression[i]);
        }

        if(!LastOpen.empty()) {
            vector<char>::iterator Symbol = find(BRACKETS_CLOSE.begin(), BRACKETS_CLOSE.end(), Expression[i]);
            
            if(Symbol != BRACKETS_CLOSE.end()) {
                if(LastOpen.top() == BRACKETS_OPEN[distance(BRACKETS_CLOSE.begin(), Symbol)]) {
                    LastOpen.pop();
                } else {
                    return false;
                }
            }
        } else {
            if(find(BRACKETS_CLOSE.begin(), BRACKETS_CLOSE.end(), Expression[i]) != BRACKETS_CLOSE.end())
                return false;
        }
    }

    return LastOpen.empty();
}

int main() {
    string Expression = string("(");

    cout << boolalpha << "Result: " << CheckBracketExpression(Expression);

    return 0;
}