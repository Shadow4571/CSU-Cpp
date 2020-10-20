#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <stdlib.h>
#include <cstring>
#include <utility>

using namespace std;

struct Line {
    double _Start;
    double _End;
    
    Line() { _Start = 0; _End = 0; }
    Line(double a, double b) { _Start = a; _End = b; }
    
    const bool operator<(const Line &Temp) const {
        return _Start < Temp._Start;
    }
    
    string ToString() {
        return "Line start: " + to_string(_Start) + " end: " + to_string(_End);
    }
};

vector<Line> FindMaxLines(vector<Line> Lines) {
    vector<Line> Result;
    sort(Lines.begin(), Lines.end());
    
    for(int i = 0; i < Lines.size(); i++) {
        if(Result.empty()) {
            Result.push_back(Lines[i]);
        } else {
            if(Result[Result.size() - 1]._End > Lines[i]._Start) {
                Result[Result.size() - 1]._End = Lines[i]._End;
            } else {
                Result.push_back(Lines[i]);
            } 
        }
    }
    
    return Result;
}

void PrintPair(pair<double, double> Temp) {
    cout << "Line start: " << Temp.first << " end: " << Temp.second << endl;
}

bool IsBiggerPair(const pair<double, double> First, const pair<double, double> Second) {
    return First.first < Second.first;
}

vector<pair<double, double>> FindMaxLines(vector<pair<double, double>> Lines) {
    vector<pair<double, double>> Result;
    sort(Lines.begin(), Lines.end(), IsBiggerPair);
    
    for(int i = 0; i < Lines.size(); i++) {
        if(Result.empty()) {
            Result.push_back(Lines[i]);
        } else {
            if(Result[Result.size() - 1].second > Lines[i].first) {
                Result[Result.size() - 1].second = Lines[i].second;
            } else {
                Result.push_back(Lines[i]);
            }
        }
    }
    
    return Result;
}

int main() {
    //vector<Line> Lines = vector<Line>{Line(0, 2), Line(1, 5), Line(-2, -1), Line(6, 8), Line(4, 7), Line(10, 15), Line(9, 13)};
    //vector<Line> UnionLines = FindMaxLines(Lines);
    
    vector<pair<double, double>> Lines = vector<pair<double, double>>{pair(0, 2), pair(1, 5), pair(-2, -1), pair(6, 8), pair(4, 7), pair(10, 15), pair(9, 13)};
    vector<pair<double, double>> UnionLines = FindMaxLines(Lines);
    
    for(auto Temp : UnionLines)
        PrintPair(Temp);
    
    return 0;
}
