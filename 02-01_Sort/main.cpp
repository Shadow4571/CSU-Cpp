#include <string>
#include <stack>
#include <Windows.h>
#include <vector>
#include <iostream>
#include <ctime>
#include <typeinfo>
#include <stdexcept>
#include <type_traits>

using std::stack;

const int default_max_count = 50000;

template <typename T>
class stackm
{
	T* m = nullptr;
	int count;
public:
	stackm(int max_count = default_max_count);
	~stackm();
	void push(const T& elem);
	void pop();
	T& top();
    bool empty();
	int size() const;
};

template <typename T>
stackm<T>::stackm(int max_count)
{
	count = 0;
	m = new T[max_count];
}

template <typename T>
stackm<T>::~stackm()
{
	delete [] m;
}

template <typename T>
void stackm<T>::pop()
{
	--count;
}

template <typename T>
void stackm<T>::push(const T& elem)
{
	m[count++] = elem;
}

template <typename T>
T& stackm<T>::top()
{
	return m[count - 1];
}

template <typename T>
bool stackm<T>::empty()
{
    return count == 0;
}

template <typename T>
int stackm<T>::size() const
{
	return count;
}

template <typename T>
class stackl
{
	struct node
	{
		T data;
		node* next;
	};
	node* Top;
	int count;
public:
	stackl();
	~stackl();
	void push(const T& elem);
	void pop();
	T& top();
	int size() const;
};

template <typename T>
stackl<T>::stackl()
{
	count = 0;
	Top = nullptr;
}

template <typename T>
void stackl<T>::push(const T& elem)
{
	++count;
	Top = new node{ elem, Top };
}

template <typename T>
void stackl<T>::pop()
{
	auto p = Top->next;
	delete Top;
	Top = p;
	--count;
}

template <typename T>
int stackl<T>::size() const
{
	return count;
}

template <typename T>
T& stackl<T>::top()
{
	return Top->data;
}

template <typename T>
stackl<T>::~stackl()
{
	while (count)
		pop();
}

template <typename Stack>
void move_top(Stack &src, Stack &dest)
{
	dest.push(src.top());
	src.pop();
}

template <typename Stack>
void move_stack(Stack &src, Stack &dest)
{
	while (src.size())
		move_top(src, dest);
}

template <typename Stack>
bool EqualStacks(Stack &s1, Stack &s2)
{
	if (s1.size() != s2.size())
		return false;
	Stack s1_copy, s2_copy;
	bool res = true;
	while (s1.size())
	{
		if (s1.top() != s2.top())
		{
			res = false;
			break;
		}
		move_top(s1, s1_copy);
		move_top(s2, s2_copy);
	}
	move_stack(s1_copy, s1);
	move_stack(s2_copy, s2);
	return res;
}

/* ======================== */
/* ==== SORT FUNCTIONS ==== */

template <typename T>
void quick_sort_stack(stackm<T> &s, int Size) {
	if (s.size() <= 1)
		return;

	stackm<T> L = stackm<T>(s.size());
	stackm<T> E = stackm<T>(s.size());
	stackm<T> G = stackm<T>(s.size());
    
	auto e = s.top();
	move_top(s, E);
    
	while (s.size())
	{
		if (s.top() > e)
			move_top(s, G);
		else if (s.top() < e)
			move_top(s, L);
		else
			move_top(s, E);
	}
	
	quick_sort_stack(L, L.size());
	quick_sort_stack(G, G.size());
    
	move_stack(G, E);
	move_stack(E, s);
	move_stack(L, G);
	move_stack(G, s);
}

template <class TStack, typename T>
void quick_sort_stack(TStack &s)
{
	if (s.size() <= 1)
		return;

	if(std::is_same<TStack, stackm<T>>::value) {
		quick_sort_stack(s, s.size());
		return;
	}

	TStack L, E, G;
    
	auto e = s.top();
	move_top(s, E);
    
	while (s.size())
	{
		if (s.top() > e)
			move_top(s, G);
		else if (s.top() < e)
			move_top(s, L);
		else
			move_top(s, E);
	}
	
	quick_sort_stack<TStack, T>(L);
	quick_sort_stack<TStack, T>(G);
    
	move_stack(G, E);
	move_stack(E, s);
	move_stack(L, G);
	move_stack(G, s);
}

template<typename T>
std::string PrintStack(T &Stack) {
    std::string Result = std::string("PRINT STACK:\n");
    
    while(Stack.size()) {
        Result += std::to_string(Stack.top()) + ' ';
        Stack.pop();
    }
    
    return Result;
}

int main()
{
    srand(time(NULL));

    stackm<int> StackArray = stackm<int>(100);
	//stackl<int> StackArray = stackl<int>();
    //stack<int> StackArray = stack<int>();	

    for(int i = 0; i < 100; i++) {
        StackArray.push(rand() % 100 + 1);
    }
    
	try {
    	quick_sort_stack<stackm<int>, int>(StackArray);
	} catch(std::invalid_argument& Exp) {
		std::cout << std::endl << "Error: " << Exp.what();
	}

	std::cout << PrintStack(StackArray) << std::endl;

	return 0;
}

