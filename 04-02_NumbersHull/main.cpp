#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>

using namespace std;

struct Point {
    double _x;
    double _y;
    
    Point() {
        _x = 0;
        _y = 0;
    }
    
    Point(double x, double y) {
        _x = x;
        _y = y;
    }
    
    double getDistance(const Point &Target) {
        return sqrt(pow(_x + Target._x, 2) + pow(_y + Target._y, 2));
    }
    
    string toString() const {
        return "Point x: " + to_string(_x) + " y: " + to_string(_y);
    }
    
    const bool operator==(const Point &T) {
        return _x == T._x && _y == T._y;
    }
    
    const Point operator+(const Point &T) {
        return Point(_x + T._x, _y + T._y);
    }
    
    const Point operator-(const Point &T) {
        return Point(_x - T._x, _y - T._y);
    }
    
    const bool operator<(const Point &T) const {
        return (_x * T._y - _y * T._x) > 0;
    }
};

void RemovePointFromSet(vector<Point> &SourceSet, const Point &Remove) {
    for(int i = 0; i < SourceSet.size(); i++) {
        if(SourceSet[i] == Remove) {
            SourceSet.erase(SourceSet.begin() + i);
            return;
        }
    }
}

vector<int> GetConvexHull(vector<Point> &P, int &Position) {
    auto p = P;
    
    auto it = min_element(p.begin(), p.end(), [](Point a, Point b){return a._y < b._y; });
    iter_swap(it, p.begin());
    
    for (int i = 1; i < p.size(); ++i)
        p[i] = p[i] - p[0];
    
    sort(p.begin() + 1, p.end());
    
    stack<Point> s;
    s.push({ 0, 0 });
    s.push(p[1]);
    int k = 2;
    while (k < p.size()) {
        auto b = s.top();
        s.pop();
        auto a = s.top();
        
        if (b - a < p[k] - b){
            s.push(b);
            s.push(p[k++]);
        }
    }
    
    vector<int> res;
    while (s.size()) {
        res.push_back(Position++);
        RemovePointFromSet(P, s.top()+p[0]);
        s.pop();
    }
    
    return res;
}

vector<vector<int>> GetCirclesFromPoints(vector<Point> PointSet) {
    vector<vector<int>> Result = vector<vector<int>>();
    
    int Start = 0;
    while (!PointSet.empty()) {
        Result.push_back(GetConvexHull(PointSet, Start));
    }
    
    return Result;
}

void PrintPointSet(const vector<int> &PointSet) {
    for(int i = 0; i < PointSet.size(); i++)
        cout <<"    Point - " << PointSet[i] << endl;
}

int main() {
    vector<Point> PointSet = vector<Point>{Point(0, 0), Point(1, 0), Point(0, 1), Point(0, 3), Point(3, 1), Point(1, -4), Point(-4, 0), Point(2, 10), Point(8, 2), Point(7, -7), Point(-4, -6), Point(-10, -2), Point(-5, 7)};
    vector<vector<int>> Circles = GetCirclesFromPoints(PointSet);
    
    for(int i = 0; i < Circles.size(); i++) {
        cout << endl << "Circle #" << i << endl;
        PrintPointSet(Circles[i]);
    }
    
    return 0;
}
