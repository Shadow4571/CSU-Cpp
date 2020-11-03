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

// Структура точки. Содержит координату x и y
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

// Структура области. Содержит крайние точки, высоту, ширину и площадь.
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

// Рекурсивная функция нахождения всех площадей, возвращает самую большую площадь
area GetContraband(area Temp, vector<area> Areas, int i, area Result) {
    // Если площадь области больше, выбираем эту область
    if(Temp.Square > Result.Square)
        Result = Temp;
    
    // Если Область не примыкает к данной области или область является последней в списке, возвращаем результат
    if(Temp.RightDown.x < Areas[i].LeftTop.x || i >= Areas.size() - 1)
        return Result;
    
    // Выбираем левую верхнюю точку и правую нижнюю точку области
    point LeftTop = Temp.LeftTop;
    point RightDown = Temp.RightDown;
    
    // Если левая верхняя точка примыкающей области ниже текущей, изменяем высоту текущей
    if(Areas[i].LeftTop.y < LeftTop.y)
        LeftTop.y = Areas[i].LeftTop.y;
    
    // Если правая нижняя точка примыкающей области выше текущей, изменяем высоту текущей
    if(Areas[i].RightDown.y > RightDown.y)
        RightDown.y = Areas[i].RightDown.y;
    
    // Продлеваем правую точку текущей области до правой примыкающей
    RightDown.x = Areas[i].RightDown.x;
    
    // Создаем новую область из текущей и примыкающей области
    Temp = area(LeftTop, RightDown);
    
    // Проверяем является ли эта площадь больше
    if(Temp.Square > Result.Square)
        Result = Temp;
    
    // Если следующих точек нету, возвращаем результат
    if(i + 1 >= Areas.size())
        return Result;
    
    // Вызываем рекурсивно функцию для следующей примыкающей области
    Result = GetContraband(Temp, Areas, i + 1, Result);
    return Result;
}

// Функция проходит по каждой области и вызывает рекурсивную функцию нахождения площади
area GetMaxArea(const vector<area> &Areas) {
    area Result = area();
    
    for(int i = 0; i < Areas.size(); i++)
        Result = GetContraband(Areas[i], Areas, i + 1, Result);
    
    return Result;
}

int main() {
    vector<area> AreaSet = vector<area>{ area(point(0, 20), point(10, 0)), area(point(10, 5), point(15, 0)), area(point(15, 30), point(20, 0)), area(point(25, 25), point(30, 0)), area(point(30, 20), point(40, 10)) };
    area Result = GetMaxArea(AreaSet);
    
    cout << "Result: " << endl << Result.toString();
    
    return 0;
}
