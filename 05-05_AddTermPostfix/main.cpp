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
enum TermPrio {P_Min = 0, P_Low = 1, P_Mid = 2, P_Hight = 3};
struct Term {
    string _data;
    TermType _type;
    TermPrio _prio;
    
    Term(string Data, TermType Type) {
        _data = Data;
        _type = Type;
        _prio = P_Min;
    }
    
    Term(string Data, TermType Type, TermPrio Prio) {
        _data = Data;
        _type = Type;
        _prio = Prio;
    }
    
    string toString() {
        return "Term data: " + _data + " type: " + to_string(_type); 
    }
};

TermPrio GetPrio(TermType Type, string Data) {
    switch(Type) {
        case T_Opbr : case T_Clbr : case T_Fun : {
            return P_Min;
        }
            
        case T_Op : {
            if(Data == "+" || Data == "-")
                return P_Low;
            
            if(Data == "*" || Data == "/")
                return P_Mid;
                
            if(Data == "^" || Data == "!")
                return P_Hight;
        }
            
        default : {
            return P_Min;
        }
    }
}

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
    
    if(SubOp.size() != 0) {
        Terms.push_back(Term(SubOp, Type, GetPrio(Type, SubOp)));
    }
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

vector<Term> GetPostfixFromTerms(const vector<Term> &Terms) {
    stack<Term> Operators;
    vector<Term> Postfix;
    
    for(const Term &Temp : Terms) {
        switch(Temp._type) {
            case T_Const : case T_Var : {
                Postfix.push_back(Temp);
                break;
            }
            
            case T_Opbr : {
                Operators.push(Temp);
                break;
            }
            
            case T_Fun : {
                Postfix.push_back(Temp);
                break;
            }
                
            case T_Clbr : {
                while(Operators.top()._type != T_Opbr) {
                    Postfix.push_back(Operators.top());
                    Operators.pop();
                }
                
                Operators.pop();
                break;
            }
                
            case T_Op : {
                if(!Operators.empty()) {
                    if(Operators.top()._prio < Temp._prio) {
                        Operators.push(Temp);
                    } else {
                        Postfix.push_back(Operators.top());
                        Operators.pop(); Operators.push(Temp);
                    }
                } else {
                    Operators.push(Temp);
                }
            }
        }
    }
    
    while(!Operators.empty()) {
        Postfix.push_back(Operators.top());
        Operators.pop();
    }
    
    return Postfix;
}

void PrintTerms(vector<Term> &Terms) {
    for(int i = 0; i < Terms.size(); i++) {
        cout << Terms[i].toString() << endl;
    }
}

int main() {
    //string Expression = "a+3.1*sin(0.5)";
    //string Expression = "3+x*(2-y)/sin(0.35)";
    string Expression = "sqrt(x)-1/2*sin(x^2-2)";
    vector<Term> Terms = GetTermExpression(Expression);
    vector<Term> Postfix = GetPostfixFromTerms(Terms);
    
    cout << "Expression: " << Expression << endl << endl;
    PrintTerms(Terms);
    cout << endl << "Postfix:" << endl;
    PrintTerms(Postfix);
    return 0;
}
