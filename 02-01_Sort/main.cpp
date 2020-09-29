#include <string>
#include <stack>
#include <Windows.h>
#include <vector>
#include <iostream>
#include <ctime>
#include <typeinfo>

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

template <typename Stack>
void dumb_sort_stack(Stack &s)
{
	if (s.size() <= 1)
		return;
	Stack res, tmp;
	while (s.size())
	{
		move_top(s, res);
		while (s.size())
			if (s.top() < res.top())
			{
				move_top(res, tmp);
				move_top(s, res);
			}
			else
				move_top(s, tmp);
		move_stack(tmp, s);
	}
	move_stack(res, s);
}

template <class TStack>
void quick_sort_stack(TStack &s, TStack L, TStack G, TStack E)
{
	if (s.size() <= 1)
		return;
    
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
    
	quick_sort_stack(L, TStack(), TStack(), TStack());
	quick_sort_stack(G, TStack(), TStack(), TStack());
    
	move_stack(G, E);
	move_stack(E, s);
	move_stack(L, G);
	move_stack(G, s);
}

template <typename T>
void quick_sort_stack(stackm<T> &s) {
	quick_sort_stack(s, stackm<T>(s.size()), stackm<T>(s.size()), stackm<T>(s.size()));
}

template <typename T>
void quick_sort_stack(stackl<T> &s) {
	quick_sort_stack(s, stackl<T>(), stackl<T>(), stackl<T>());
}

template <typename T>
void quick_sort_stack(stack<T> &s) {
	quick_sort_stack(s, stack<T>(), stack<T>(), stack<T>());
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

template <typename F>
int exec_time(F f)
{
	auto start = GetTickCount();
	f();
	return GetTickCount() - start;
}

int main()
{
	//std::vector<int> numbers{ 12, 4, 5, 9, 1, 0, 7, 3, 8, 1, 2, 9, 7, 5, 3, 0, 6 };
//	std::vector<int> numbers;
//	for (int i = 0; i < 100; ++i)
//		numbers.push_back(i);
//
//	stackm<int> sm;
//	stackl<int> sl;
//	std::stack<int> s;
//	for (auto e : numbers)
//	{
//		sm.push(e);
//		sl.push(e);
//		s.push(e);
//	}
    srand(time(NULL));

    //stackm<int> StackArray = stackm<int>(100);
	//stackl<int> StackArray = stackl<int>();
    stack<int> StackArray = stack<int>();

    for(int i = 0; i < 100; i++) {
        StackArray.push(rand() % 100 + 1);
    }
    
    quick_sort_stack(StackArray);
    
    std::cout << PrintStack(StackArray) << std::endl;
    
//	std::cout << "stackm: " << exec_time([&](){quick_sort_stack(StackArray); }) << std::endl;
//	std::cout << "stackl: " << exec_time([&](){quick_sort_stack(sl); }) << std::endl;
//	std::cout << "stack : " << exec_time([&](){quick_sort_stack(s); }) << std::endl;
	
	//quick_sort_stack(sm);
	//quick_sort_stack(sl);
	//quick_sort_stack(s);

	//std::cout << std::endl;
	//while (sl.size())
	//{
	//	std::cout << sl.top() << " ";
	//	sl.pop();
	//}
	//std::cout << std::endl;
	//while (s.size())
	//{
	//	std::cout << s.top() << " ";
	//	s.pop();
	//}
	//std::cout << std::endl;
	return 0;
}

