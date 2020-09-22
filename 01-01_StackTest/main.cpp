#include <iostream>
#include <stack>
#include <windows.h>

using namespace std;

struct PerfomanceTest {
    double PCFreq = 0.0;
    __int64 CounterStart = 0;

    void StartPerfomanceTest() {
        LARGE_INTEGER StartTime;
        if(!QueryPerformanceFrequency(&StartTime))
            cout << "Use Windows for perfomance test";

        PCFreq = (double)(StartTime.QuadPart / 1000);

        QueryPerformanceCounter(&StartTime);
        CounterStart = StartTime.QuadPart;
    }

    double StopPerfomanceTest() {
        LARGE_INTEGER StopTime;
        QueryPerformanceCounter(&StopTime);

        return (double)(StopTime.QuadPart - CounterStart) / PCFreq;
    }
};

pair<string, double> StackProbabilityTest(stack<int> *UserStack, double Probability) {
    pair<string, double> Result = pair<string, double>();
    PerfomanceTest Tester = PerfomanceTest();

    Result.first = "Пользовательский стэк размера: " + to_string(UserStack->size()) + "\n" + "Вероятность появления элемента: " + to_string(Probability);

    Tester.StartPerfomanceTest();
    for(long i = 0; i < 200000000; i++) {
        double Random = (double)(rand() / (double)RAND_MAX);

        if(Probability <= Random)
            UserStack->push((int)i);

        if(Probability <= (1.0 - Random) && !UserStack->empty())
            UserStack->pop();
    }

    Result.second = Tester.StopPerfomanceTest();

    return Result;
}

int main() {
    stack<int> UserStack = stack<int>();
    double Probability = 0.3;
    
    cout << "Введите вероятность: ";
    cin >> Probability;

    pair<string, double> Result = StackProbabilityTest(&UserStack, Probability);

    cout << endl << Result.first + "\n" + "Было затрачено времени: " + to_string(Result.second) + " милисекунд" << endl;
    cout << "Стэк содержит: " + to_string(UserStack.size()) + " элементов";

    return 0;
}