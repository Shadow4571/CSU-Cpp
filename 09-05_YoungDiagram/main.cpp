#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <stdlib.h>
#include <cstring>
#include <utility>
#include <iomanip>

using namespace std;

// Функция возвращает строку возрастающих значений {1, 2, 3, 2} - {1, 2 ,3}
vector<int> getIncreasingElements(vector<int> Array) {
    for(int i = 0; i < Array.size() - 1; i++)
        if(Array[i] > Array[i + 1])
            return vector<int>(Array.begin(), Array.begin() + i + 1);
    
    return Array;
}

// Функция проверяет строки, чтобы нижняя строка содержала большие значения
vector<int> getAfterCompare(vector<int> First, vector<int> Second) {
    for(int i = 0; i < Second.size(); i++)
        if(First[i] > Second[i])
            return vector<int>(Second.begin(), Second.begin() + i);
    
    return Second;
}

// Функция возвращает диаграмму Юнга
vector<vector<int>> getYoungDiagram(vector<vector<int>> Matrix) {
    vector<vector<int>> Result = vector<vector<int>>();
    
    for(int i = 0; i < Matrix.size(); i++)
        Result.push_back(getIncreasingElements(Matrix[i]));
    
    for(int i = 0; i < Result.size() - 1; i++)
        Result[i + 1] = getAfterCompare(Result[i], Result[i + 1]);
    
    return Result;
}

// Функция печатает матрицу в консоль
void printMatrix(vector<vector<int>> Matrix) {
    cout << endl << "Matrix:" << endl;
    for(int i = 0; i < Matrix.size(); i++) {
        for(int j = 0; j < Matrix[i].size(); j++) {
            string Space = Matrix[i][j] > 9 ? "   " : "    ";
            cout << Matrix[i][j] << Space;
        }
        cout << endl;
    }
}

int main() {
    vector<vector<int>> Matrix  =   {{ 1, 2, 3, 4, 5 },
                                    { 6, 7, 8, 9, 3 },
                                    { 10, 20, 25, 1, 2 },
                                    { 12, 21, 22, 1, 1 },
                                    { 1, 2, 0, 0, 0 } };
    
    vector<vector<int>> Result = getYoungDiagram(Matrix);
	
    printMatrix(Matrix);
    printMatrix(Result);
    
    return 0;
}
