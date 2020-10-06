#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

int FindIncorrectBracket(string Expression) {
    vector<char> BRACKETS_OPEN = vector<char>({'(', '{', '[', '<'});
    vector<char> BRACKETS_CLOSE = vector<char>({')', '}', ']', '>'});

    stack<char> LastOpen = stack<char>();
    stack<int> LastPosition = stack<int>();

    for(int i = 0; i < Expression.size(); i++) {
        if(find(BRACKETS_OPEN.begin(), BRACKETS_OPEN.end(), Expression[i]) != BRACKETS_OPEN.end()) {
            LastOpen.push(Expression[i]);
            LastPosition.push(i);
        }

        if(!LastOpen.empty()) {
            vector<char>::iterator Symbol = find(BRACKETS_CLOSE.begin(), BRACKETS_CLOSE.end(), Expression[i]);
            
            if(Symbol != BRACKETS_CLOSE.end()) {
                if(LastOpen.top() == BRACKETS_OPEN[distance(BRACKETS_CLOSE.begin(), Symbol)]) {
                    LastOpen.pop();
                    LastPosition.pop();
                } else {
                    return i;
                }
            }
        } else {
            if(find(BRACKETS_CLOSE.begin(), BRACKETS_CLOSE.end(), Expression[i]) != BRACKETS_CLOSE.end())
                return i;
        }
    }

    return LastPosition.empty() ? -1 : LastPosition.top();
}

int main() {
    string Expression = "((){}()])";
    
    cout << "Result: " << FindIncorrectBracket(Expression);

    return 0;
}