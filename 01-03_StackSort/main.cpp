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
void StackSort(stack<T> &OriginalStack) {
    stack<T> SortedStack = stack<T>();
    bool IsUnSorted = true;
    
    while(IsUnSorted) {
        stack<T> Temp = stack<T>(OriginalStack);
        
        if(SortedStack.size() == OriginalStack.size()) {
            IsUnSorted = false;
        } else {
            T Max = NULL;
            
            while(!Temp.empty()) {
                T Element = Temp.top();
                Temp.pop();
                
                if(SortedStack.empty()) {
                    if(Max == NULL){
                        Max = Element;
                    } else {
                        if(Max < Element)
                            Max = Element;
                    }
                } else {
                    if(Max == NULL) {
                        if(Element < SortedStack.top())
                            Max = Element;
                    } else {
                        if(Element < SortedStack.top() && Max < Element)
                            Max = Element;
                    }
                }
            }
            
            SortedStack.push(Max);
        }
    }
    
    OriginalStack = SortedStack;
}

int main() {
    stack<int> UnSortedStack = stack<int>(deque<int> {1, 3, 2, 5, 4});
    PrintStack(UnSortedStack);
    
    StackSort(UnSortedStack);
    
    cout << endl;
    PrintStack(UnSortedStack);
    
    return 0;
}
