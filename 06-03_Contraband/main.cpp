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

struct point {
    double x;
    double y;
    
    point() { this->x = 0; this->y = 0; }
    
    point(double x, double y) { this->x = x; this->y = y; }
    
    point(const point &t) { this->x = t.x; this->y = t.y; }
    
    string toString() {
        return "x: " + to_string(x) + " y: " + to_string(y);
    }
};

struct area {
    point LeftTop;
    point RightDown;
    double Width;
    double Height;
    double Square;
    
    area() {
        this->LeftTop = point(); this->RightDown = point();
        this->Width = 0; this->Height = 0;
        this->Square = 0;
    }
    
    area(point LeftTop, point RightDown) {
        this->LeftTop = point(LeftTop);
        this->RightDown = point(RightDown);
        this->Width = this->RightDown.x - this->LeftTop.x;
        this->Height = this->LeftTop.y - this->RightDown.y;
        this->Square = this->Width * this->Height;
    }
    
    string toString() {
        return "Area left point: " + this->LeftTop.toString() + " right point: " + this->RightDown.toString() + '\n' +
            "   Width: " + to_string(this->Width) + " Height: " + to_string(this->Height) + " | Square: " + to_string(this->Square) + '\n';
    }
};

void PrintAreas(const vector<area> &Areas) {
    for(area i : Areas)
        cout << i.toString() << endl;
}

area GetMaxArea(vector<area> Areas) {
    vector<area> AllAreas = vector<area>();
    area Result = area();
    
    for(long long unsigned int i = 0; i < Areas.size(); i++) {
        AllAreas.push_back(Areas[i]);;
        
        if(Areas[i].Square > Result.Square)
            Result = Areas[i];
        
        if(i != Areas.size() - 1) {
            point LeftTop = Areas[i].LeftTop;
            point RightDown = Areas[i].RightDown;
            for(long long unsigned int j = i + 1; j < Areas.size(); j++) {
                if(Areas[j].LeftTop.x != RightDown.x)
                    break;
                
                if(Areas[j].LeftTop.y < LeftTop.y)
                    LeftTop.y = Areas[j].LeftTop.y;
                
                if(Areas[j].RightDown.y > RightDown.y)
                    RightDown.y = Areas[j].RightDown.y;
                
                RightDown.x = Areas[j].RightDown.x;
                AllAreas.push_back(area(LeftTop, RightDown));
                
                if(AllAreas[AllAreas.size() - 1].Square > Result.Square)
                    Result = AllAreas[AllAreas.size() - 1];
            }
        }
    }
    
    return Result;
}

int main() {
    vector<area> AreaSet = vector<area>{ area(point(0, 20), point(10, 0)), area(point(10, 5), point(15, 0)), area(point(15, 30), point(20, 0)), area(point(25, 25), point(30, 0)), area(point(30, 20), point(40, 10)) };
    area Result = GetMaxArea(AreaSet);
    
    cout << "Result: " << endl << Result.toString();
    
    return 0;
}
