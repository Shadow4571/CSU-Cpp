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

vector<Point> convex_hull(const vector<Point> &P) {
	if (P.size() < 4)
		return P;

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
		res.push_back(s.top()+p[0]);
		s.pop();
	}

	return res;
}

void RemovePointFromSet(vector<Point> &SourceSet, const vector<Point> &Remove) {
    for(int i = 0; i < Remove.size(); i++) {
        for(int j = 0; j < SourceSet.size(); j++) {
            if(SourceSet[j] == Remove[i]) {
                SourceSet.erase(SourceSet.begin() + j, SourceSet.begin() + j + 1);
            }
        }
    }
}

vector<vector<Point>> GetCirclePoint(vector<Point> PointSet) {
    vector<vector<Point>> Result = vector<vector<Point>>();

    while (!PointSet.empty()) {
        Result.push_back(convex_hull(PointSet));
        RemovePointFromSet(PointSet, Result[Result.size() - 1]);
    }

    return Result;
}

void PrintPointSet(const vector<Point> &PointSet) {
    for(int i = 0; i < PointSet.size(); i++)
        cout << i << " - " << PointSet[i].toString() << endl;
}

int main() {
    vector<Point> PointSet = vector<Point>{Point(0, 0), Point(1, 0), Point(0, 1), Point(0, 3), Point(3, 1), Point(1, -4), Point(-4, 0), Point(2, 10), Point(8, 2), Point(7, -7), Point(-4, -6), Point(-10, -2), Point(-5, 7)};
    vector<vector<Point>> Circles = GetCirclePoint(PointSet);

    for(int i = 0; i < Circles.size(); i++) {
        cout << endl << "Circle #" << i << endl;
        PrintPointSet(Circles[i]);
    }

    return 0;
}