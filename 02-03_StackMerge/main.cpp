#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

template<typename T>
stack<T> StackMerge(stack<T> FirstStack, stack<T> SecondStack) {
    stack<T> Result = stack<T>();

    stack<T> *First = &FirstStack;
    stack<T> *Second = &SecondStack;
    
    if(FirstStack.top() > SecondStack.top()) {
        First = &SecondStack;
        Second = &FirstStack;
    }

    while(!First->empty()) {
        Result.push(First->top());
        First->pop();
        
        if(!First->empty()) {
            if(First->top() >= Second->top()) {
                Result.push(Second->top());
                Second->pop();
            }
        }
    }

    while(!Second->empty()) {
        Result.push(Second->top());
        Second->pop();
    }

    return Result;
}

int main() {
    stack<int> First = stack<int>(deque<int> {13, 9, 7, 3, 1});
    stack<int> Second = stack<int>(deque<int> {12, 6, 2});

    stack<int> Result = StackMerge(First, Second);

    while(!Result.empty()) {
        cout << Result.top() << endl;
        Result.pop();
    }

    return 0;
}