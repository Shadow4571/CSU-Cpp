#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>

using namespace std;

struct Point {
    int _number;
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
        return "Point - " + to_string(_number) + " with x: " + to_string(_x) + " y: " + to_string(_y);
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

vector<Point> GetConvexHull(vector<Point> &P, int &Position) {
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
    
    vector<Point> res;
    while (s.size()) {
        res.push_back(s.top() + p[0]);
        res[res.size() - 1]._number = Position++;
        RemovePointFromSet(P, res[res.size() - 1]);
        s.pop();
    }
    
    return res;
}

vector<vector<Point>> GetCirclesFromPoints(vector<Point> PointSet) {
    vector<vector<Point>> Circles = vector<vector<Point>>();
    
    int Start = 0;
    while (!PointSet.empty()) {
        Circles.push_back(GetConvexHull(PointSet, Start));
    }
    
    return Circles;
}

void PrintPointSet(const vector<Point> &PointSet) {
    for(int i = 0; i < PointSet.size(); i++)
        cout << PointSet[i].toString() << endl;
}

int main() {
    vector<Point> PointSet = vector<Point>{Point(0, 0), Point(1, 0), Point(0, 1), Point(0, 3), Point(3, 1), Point(1, -4), Point(-4, 0), Point(2, 10), Point(8, 2), Point(7, -7), Point(-4, -6), Point(-10, -2), Point(-5, 7)};
    vector<vector<Point>> Circles = GetCirclesFromPoints(PointSet);
    
    for(int i = 0; i < Circles.size(); i++) {
        cout << endl << "Circle #" << i << endl;
        PrintPointSet(Circles[i]);
    }
    
    return 0;
}
