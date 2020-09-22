#include <iostream>
#include <stack>

using namespace std;

template<typename T>
void PrintStack(stack<T> CurrentStack) {
    cout << "Stack size: " << CurrentStack.size() << endl;
    while(!CurrentStack.empty()) {
        cout << CurrentStack.top() << endl;
        CurrentStack.pop();
    }
}

template<typename T>
stack<T> StackTransfer(stack<T> OriginalStack) {
    stack<T> NewStack = stack<T>();
    
    while(NewStack.size() != OriginalStack.size()) {
        stack<T> Temp = stack<T>(OriginalStack);
        T Min = NULL;
        
        while(!Temp.empty()) {
            T Element = Temp.top();
            Temp.pop();
            
            if(NewStack.empty()) {
                if(Min == NULL) {
                    Min = Element;
                } else {
                    if(Element < Min)
                        Min = Element;
                }
            } else {
                if(Min == NULL) {
                    if(Element > NewStack.top())
                        Min = Element;
                } else {
                    if(Element > NewStack.top() && Min > Element)
                        Min = Element;
                }
            }
        }
        
        NewStack.push(Min);
    }
    
    return NewStack;
}

int main() {
	stack<int> UnSortedStack = stack<int>(deque<int> {5, 2, 3, 1, 4});
    stack<int> SortedStack = StackTransfer(UnSortedStack);
    
    PrintStack(UnSortedStack);
    cout << endl;
    PrintStack(SortedStack);
    
    return 0;
}
