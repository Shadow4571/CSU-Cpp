#include <iostream>
#include <sstream>
#include <stack>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <stdlib.h>
#include <cstring>

using namespace std;

struct variable {
    string symbol;
    double value;
    
    variable(string Expression) {
        char *End;
        this->symbol = Expression.substr(0, Expression.find('='));
        this->value = strtod(Expression.substr(Expression.find('=') + 1, Expression.size()).c_str(), &End);
    }
    
    string toString() {
        return this->symbol + " - " + to_string(this->value);
    }
};

vector<string> SplitString(const string &Str) {
    vector<string> Result = vector<string>();
    stringstream Stream = stringstream(Str);
    string Item;
    
    while(getline(Stream, Item, '\n'))
        Result.push_back(Item);
    
    return Result;
}

vector<variable> GetVariables(const vector<string> &SplittedExpression) {
    vector<variable> Result = vector<variable>();
    
    for(string i : SplittedExpression)
        if(i.find('=') != string::npos)
            Result.push_back(variable(i));
    
    return Result;
}

int main() {
    string Expression = "x+y\nx=2\ny=5";
    vector<string> SplittedExpression = SplitString(Expression);
    
    for(variable i : GetVariables(SplittedExpression))
        cout << i.toString() << endl;
    
    return 0;
}
