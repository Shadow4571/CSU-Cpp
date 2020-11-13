#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <stdlib.h>
#include <cstring>
#include <utility>
#include "term.h"

using namespace std;

// Функция проверяет вектора на равенство. Когда все элементы стоят в одинаковом порядке
template<typename T>
bool vectorsIsEquals(vector<T> &first, vector<T> &second) {
    if(first.size() != second.size())
        return false;
    
    for(int i = 0; i < first.size(); i++)
        if(first[i] != second[i])
            return false;
    
    return true;
} 

// Функция находит наибольшой результат, перебирая всевозможные комбинации операторов
double findBestOperatorsCombination(vector<double> numbers, vector<char> op, vector<char> *temp = nullptr, int i = 0, double result = 0) {
	// Если временный вектор операторов пуст, создаем дубликат из начальных операторов
    if(temp == nullptr)
        temp = new vector<char>(op);
    
	// Записываем выражение в строку
    string expression;
    for(int j = 0; j < numbers.size(); j++) {
        expression += to_string(numbers[j]);
        if(j < numbers.size() - 1)
            expression += temp->at(j);
    }
    
	// Меняем местами операторы, например было (* / -) станет (/ * -) потом (/ - *), (- / *), (- * /), (* - /), (* / -)
    iter_swap(temp->begin() + i, temp->begin() + i + 1);
    i = i < numbers.size() - 3 ? i + 1 : 0;
    
	// Вычисляем значение варажения из строки, используя калькулятор термов
    double tempResult = CalculateFromTerms(GetPostfixFromTerms(GetTermExpression(expression)));
	// Если результат получился больше, сохраняем его
    result = tempResult > result ? tempResult : result;
    
	// Если после перестановки местами операторы не вернулись в изначальное положение, вызываем функцию
    if(!vectorsIsEquals<char>(op, *temp))
        result = findBestOperatorsCombination(numbers, op, temp, i, result);
    
    return result;
}

int main() {
    vector<char> op = {'*', '/', '-'};
    vector<double> numbers = vector<double>{1, 3, 0.1, 1};
    
    cout << "Result: " << findBestOperatorsCombination(numbers, op);
    
    return 0;
}
