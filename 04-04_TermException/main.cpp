#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <stdlib.h>
#include <cstring>

using namespace std;

const vector<string> OPERATORS_TYPE {"+", "-", "*", "/", "^", "!"};
const vector<string> OPEN_BRACKETS_TYPE {"("};
const vector<string> CLOSE_BRACKETS_TYPE {")"};
const vector<string> FUNCTIONS_TYPE {"sin", "cos", "tg", "ctg", "asin", "acos", "atg", "actg"};

enum TermType {T_Var, T_Const, T_Op, T_Fun, T_Clbr, T_Opbr};
struct Term {
    string _data;
    TermType _type;
    
    Term(string Data, TermType Type) {
        _data = Data;
        _type = Type;
    }
    
    string toString() {
        return "Term data: " + _data + " type: " + to_string(_type); 
    }
};

int FindFromTypes(vector<string> Types, const string &From, int Position) {
    for(int i = 0; i < Types.size(); i++) {
        //cout << endl << "COMPARE: " << From.substr(Position, Types[i].size()) << " WITH: " << Types[i] << endl;
        if(strcmp(From.substr(Position, Types[i].size()).c_str(), Types[i].c_str()) == 0) {
            return Position + Types[i].size();
        }
    }
    
    return -1;
}

bool CheckStringIsNumber(string Str) {
    char *resultstr;
    strtod(Str.c_str(), &resultstr);
    
    return resultstr[0] == NULL;
}

Term GetVariableOrConst(const string &Str) {
    if(CheckStringIsNumber(Str)) {
        return Term(Str, T_Const);
    }
    
    return Term(Str, T_Var);
}

void AddTermToResult(vector<Term> &Terms, const string &SubVar, const string &SubOp, TermType Type) {
    if(SubVar.size() != 0)
        Terms.push_back(GetVariableOrConst(SubVar));
    
    if(SubOp.size() != 0)
        Terms.push_back(Term(SubOp, Type));
}

vector<Term> GetTermExpression(const string &Expression) {
    vector<Term> Result = vector<Term>();
    
    string VariableOrConst = "";
    int Start = 0; int End = 0;
    
    for(int i = 0; i < Expression.size(); i++) {
        int Find = FindFromTypes(OPERATORS_TYPE, Expression, End);
        if(Find != -1) {
            string SubVar = Expression.substr(Start, End - Start);
            string SubOp = Expression.substr(End, Find - i);
            AddTermToResult(Result, SubVar, SubOp, T_Op);
            Start = Find;
        }
        
        Find = FindFromTypes(OPEN_BRACKETS_TYPE, Expression, End);
        if(Find != -1) {
            string SubVar = Expression.substr(Start, End - Start);
            string SubOp = Expression.substr(End, Find - i);
            AddTermToResult(Result, SubVar, SubOp, T_Opbr);
            Start = Find;
        }
        
        Find = FindFromTypes(CLOSE_BRACKETS_TYPE, Expression, End);
        if(Find != -1) {
            string SubVar = Expression.substr(Start, End - Start);
            string SubOp = Expression.substr(End, Find - i);
            AddTermToResult(Result, SubVar, SubOp, T_Clbr);
            Start = Find;
        }
        
        Find = FindFromTypes(FUNCTIONS_TYPE, Expression, End);
        if(Find != -1) {
            string SubVar = Expression.substr(Start, End - Start);
            string SubOp = Expression.substr(End, Find - i);
            AddTermToResult(Result, SubVar, SubOp, T_Fun);
            Start = Find;
        }
        
        End++;
        if(End == Expression.size()) {
            string SubVar = Expression.substr(Start, End - Start);
            AddTermToResult(Result, SubVar, "", T_Fun);
        }
    }
    
    return Result;
}

void PrintTerms(vector<Term> &Terms) {
    for(int i = 0; i < Terms.size(); i++) {
        cout << Terms[i].toString() << endl;
    }
}

int main() {
    string Expression = "a+3.1*sin(0.5)";
    //string Expression = "3+x*(2-y)/sin(0.35)";
    vector<Term> Terms = GetTermExpression(Expression);
    
    cout << "Expression: " << Expression << endl << endl;
    PrintTerms(Terms);
    return 0;
}
