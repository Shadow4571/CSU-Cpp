#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

void PrintGraph(const vector<vector<int>> &AdjacencyMatrix) {
    for(int i = 0; i < AdjacencyMatrix.size(); i++) {
        cout << "Point: " << i << endl ;
        for(int j = 0; j < AdjacencyMatrix[i].size(); j++)
            if(AdjacencyMatrix[i][j] == 1)
                cout << "   Path to -> " << j << endl;
    }
}

void AddEdge(vector<vector<int>> &Graph, int Start, int End, int Number, bool IsTwoSide) {
    Graph[Start][End] = Number;

    if(IsTwoSide)
        Graph[End][Start] = Number;
}

vector<vector<int>> CreateEmptyGraph(uint16_t VertexCount) {
    vector<vector<int>> Graph = vector<vector<int>>(VertexCount);

    for(int i = 0; i < VertexCount; i++)
        for(int j = 0; j < VertexCount; j++)
            Graph[i].push_back(0);

    return Graph;
}

vector<vector<int>> CreateGraph(const uint16_t &VertexCount, const uint16_t &EdgeCount, const bool &IsTwoSide) {
    if(VertexCount < 1 || (VertexCount > 0 && EdgeCount < VertexCount - 1))
        throw "Graph cannot have unconnected vertices or contain 0 vertices.";

    if(EdgeCount < VertexCount)
        throw "Edge count must be greater then: " + to_string(VertexCount);

    vector<vector<int>> Graph = CreateEmptyGraph(VertexCount);
    srand(time(NULL));

    int i = 0;
    int Max = (int)EdgeCount;

    while(Max > 0) {
        int RandPosition1, RandPosition2;

        do {
            RandPosition1 = rand() % VertexCount;
            RandPosition2 = rand() % VertexCount;
        } while(RandPosition1 == i || RandPosition2 == i);

        AddEdge(Graph, i, RandPosition1, 1, IsTwoSide);

        Max--;
        if(IsTwoSide)
            Max--;

        if(++i >= VertexCount)
            i = 0;
    }

    return Graph;
}

int main() {
    vector<vector<int>> Graph;

    try {
        Graph = CreateGraph(10, 25, false);
    } catch(const char* Exp) {
        cout << Exp;
    }

    PrintGraph(Graph);

    return 0;
}