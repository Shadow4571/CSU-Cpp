#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

template<typename T>
bool EqualStacks(stack<T> FirstCompare, stack<T> SecondCompare) {
    if(FirstCompare.size() != SecondCompare.size())
        return false;

    vector<T> FirstVector = vector<T>();
    vector<T> SecondVector = vector<T>();

    while(!FirstCompare.empty()) {
        FirstVector.insert(FirstVector.begin(), FirstCompare.top());
        FirstCompare.pop();

        SecondVector.insert(SecondVector.begin(), SecondCompare.top());
        SecondCompare.pop();
    }

    for(int i = 0; i < FirstVector.size(); i++)
        if(find(SecondVector.begin(), SecondVector.end(), FirstVector[i]) == SecondVector.end())
            return false;

    return true;
}

int main() {
    stack<int> First = stack<int>(deque<int> {1, 2, 3, 4, 5});
    stack<int> Second = stack<int>(deque<int> {5, 4, 3, 2, 1});

    cout << boolalpha << "Result: " << EqualStacks<int>(First, Second);
    return 0;
}
