#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

void PrintGraph(const vector<vector<int>> &AdjacencyMatrix) {
    for(int i = 0; i < AdjacencyMatrix.size(); i++) {
        cout << "Point: " << i << endl ;
        for(int j = 0; j < AdjacencyMatrix[i].size(); j++)
            if(AdjacencyMatrix[i][j] == 1)
                cout << "   Path to -> " << j << endl;
    }
}

void PrintPath(stack<int> Path) {
    cout << endl << "Path: ";
    while(!Path.empty()) {
        cout << Path.top();
        Path.pop();

        if(!Path.empty())
            cout << " -> ";
    }
    
    cout << endl;
}

bool CheckGraph(vector<vector<int>> AdjacencyMatrix) {
    int OddCount = 0;

    for(int i = 0; i < AdjacencyMatrix.size(); i++) {
        if(count(AdjacencyMatrix[i].begin(), AdjacencyMatrix[i].end(), 1) % 2 != 0 ) {
            OddCount++;
        }
    }

    return OddCount == 0 || OddCount == 2;
}

void RemoveEdge(vector<vector<int>> &AdjacencyMatrix, int Point, int End) {
    AdjacencyMatrix[Point][End] = 0;
    AdjacencyMatrix[End][Point] = 0;
}

int GraphTraverse(vector<vector<int>> &AdjacencyMatrix, int Point, stack<int> &Result) {
    for(int i = 0; i < AdjacencyMatrix[Point].size(); i++) {
        if(AdjacencyMatrix[Point][i] == 1) {
            Result.push(i);
            RemoveEdge(AdjacencyMatrix, Point, i);
            Point = i;
        }
    }

    return Point;
}

stack<int> FindGraphPath(vector<vector<int>> AdjacencyMatrix, int StartPoint) {
    stack<int> Result = stack<int>();

    if(!CheckGraph(AdjacencyMatrix))
        throw "Graph is incorrect: (May contains 0 or 2 odd point)";

    Result.push(StartPoint);
    GraphTraverse(AdjacencyMatrix, StartPoint, Result);
    
    return Result;
}

int main() {
    vector<vector<int>> AdjacencyMatrix = vector<vector<int>>{{0, 1, 0, 1, 1}, {1, 0, 1, 0, 0}, {0, 1, 0, 1, 1}, {0, 0, 1, 0, 1}, {1, 0, 0, 1, 0}};
    stack<int> Result;

    try {
        Result = FindGraphPath(AdjacencyMatrix, 0);
    } catch(const char *Exp) {
        cout << "Error: " << Exp;
    }

    PrintPath(Result);

    return 0;
}