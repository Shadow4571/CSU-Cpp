#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <utility>
#include <iomanip>

using namespace std;

double fastPow(double number, int n) {
    if(n == 0)
        return 1;
    if(n % 2 == 1) {
        return fastPow(number, n - 1) * number;
    } else {
        double b = fastPow(number, n / 2);
        return b * b;
    }
}

int main() {
    cout << fixed << setprecision(0) << "2^2=" << fastPow(2, 2) << " 3^3=" << fastPow(3, 3) << " 5^3=" << fastPow(5, 3) << " 2^42=" << fastPow(2, 42) << " 6^27=" << fastPow(6, 27);
    
    return 0;
}
