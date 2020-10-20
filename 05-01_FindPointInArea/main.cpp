#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <stdlib.h>
#include <cstring>

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
        return "Point with x: " + to_string(_x) + " y: " + to_string(_y);
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

struct Area {
    vector<Point> _Points;
    
    Area(vector<Point> Points) {
        _Points = Points;
    }
};

double GetRotate(Point A, Point B, Point C) {
    return (B._x - A._x) * (C._y - B._y) - (B._y - A._y) * (C._x - B._x);
}

bool GetIntersect(Point A, Point B, Point C, Point D) {
    return GetRotate(A, B, C) * GetRotate(A, B, D) <= 0 && GetRotate(C, D, A) * GetRotate(C, D, B) < 0;
}

bool GetPointLocation(Area Polygon, Point A) {
    int n = Polygon._Points.size();
    
    if(GetRotate(Polygon._Points[0], Polygon._Points[1], A) < 0 || GetRotate(Polygon._Points[0], Polygon._Points[n - 1], A) > 0)
        return false;
    
    int p = 1;
    int r = n - 1;
    
    while(r - p > 1) {
        int q = (p + r) / 2;
        
        if(GetRotate(Polygon._Points[0], Polygon._Points[q], A) < 0)
            r = q;
        else
            p = q;
    }
    
    return !GetIntersect(Polygon._Points[0], A, Polygon._Points[p], Polygon._Points[r]);
}

int main() {
    Area Polygon = Area( vector<Point>{Point(0, 0), Point(1, - 3), Point(3, -5), Point(6, -2), Point(5, 3), Point(2, 6), Point(1, 2)} );
    vector<Point> PointSet = vector<Point>{Point(2, 0)};
    
    cout << boolalpha << GetPointLocation(Polygon, PointSet[0]);
    
    return 0;
}
