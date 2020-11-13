#pragma once
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <iostream>
#include <stdlib.h>
#include <utility>
#include <math.h>
#include <cstring>

using namespace std;

#ifndef TERM_H
#define TERM_H

const vector<string> OPERATORS_TYPE {"+", "-", "*", "/", "^", "!"};
const vector<string> OPEN_BRACKETS_TYPE {"("};
const vector<string> CLOSE_BRACKETS_TYPE {")"};
const vector<string> FUNCTIONS_TYPE {"sin", "cos", "tg", "ctg", "asin", "acos", "atg", "actg", "sqrt"};

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

TermPrio GetPrio(TermType Type, string Data);

int FindFromTypes(vector<string> Types, const string &From, int Position);

bool CheckStringIsNumber(string Str);

Term GetVariableOrConst(const string &Str);

void AddTermToResult(vector<Term> &Terms, const string &SubVar, const string &SubOp, TermType Type);

vector<Term> GetTermExpression(const string &Expression);

vector<Term> GetPostfixFromTerms(const vector<Term> &Terms);

double CalculateFromTerms(vector<Term> Postfix);

#endif
