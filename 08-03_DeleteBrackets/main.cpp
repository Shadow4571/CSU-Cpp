#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <iostream>
#include <stdlib.h>
#include <utility>
#include <math.h>

using namespace std;

enum term_t
{
    t_var,//0
    t_const,//1
    t_op,//2
    t_fun,//3
    t_opbr,//4
    t_clbr//5
};

struct term
{
    string name;
    term_t type;
    bool isMinus = false;
};

vector<term> get_terms_from_expression(const string &s)
{
    vector<term> res;
    for (int i = 0; i < s.size(); i++)
    {
        if (isdigit(s[i]))
        {
            string consts;
            while (s[i])
            {
                consts += s[i];
                i++;
            }
            res.push_back(term{ consts, t_const });
            i--;
            continue;
        }
        if (s[i] >= 'a' && s[i] <= 'z')
        {
            if (s[i + 1] < 'a' || s[i + 1] > 'z')
            {
                res.push_back(term{ string{ s[i] }, t_var });
                continue;
            }
            if (s[i + 1] >= 'a' && s[i + 1] <= 'z')
            {
                string funs;
                while (s[i] >= 'a' && s[i] <= 'z')
                {
                    funs += s[i];
                    i++;
                }
                i--;
                res.push_back(term{ funs, t_fun });
                continue;
            }
        }
        if (s[i] == '+' || s[i] == '-' || s[i] == '/' || s[i] == '*' || s[i] == '^' || s[i] == '!')
        {
            res.push_back(term{ string{ s[i] }, t_op });
            continue;
        }
        if (s[i] == '(')
        {
            res.push_back(term{ string{ s[i] }, t_opbr });
            continue;
        }
        if (s[i] == ')')
        {
            res.push_back(term{ string{ s[i] }, t_clbr });
            continue;
        }
    }
    return res;
}

const string delimiters = "~!@#$%^&*() +-*/";
bool isdelimiter(char c)
{
    return std::find(begin(delimiters), end(delimiters), c) != end(delimiters);
}

vector<term> ya_get_terms_from_expression(string s)
{
    vector<term> res;
    int i = 0;
    while (i < s.length())
    {
        if (s[i] == ' ')
        {
            ++i; continue;
        }
        string trm = string(1, s[i]);
        while (!isdelimiter(s[i]) && i + 1 < s.length() && !isdelimiter(s[i + 1]))
            trm += s[++i];
        res.push_back({ trm, t_op });
        ++i;
    }
    
    bool setMinus = false;
    for (int i = 0; i < res.size(); ++i)
    {
        char c = res[i].name[0];
        
        if (isdigit(c)) {
            res[i].type = t_const;
        } else if (c == '(') {
            res[i].type = t_opbr;
        } else if (c == ')') {
            res[i].type = t_clbr;
        } else if (isalpha(c)) {
            if (i < res.size() - 1 && res[i + 1].name == "(") {
                res[i].type = t_fun;
            } else {
                res[i].type = t_var;
            }
        } else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
            res[i].type = t_op;
        }
    }
    return res;
}

const int MAX_PRIO = 30;
std::map<string, int> priority{ { "+", 10 }, { "-", 10 }, { "*", 20 }, { "/", 20 }, { "^", 30 }, { "!", 25 } };

vector<term> get_postfix_from_terms(vector<term> terms)
{
    std::stack<term> s;
    std::vector<term> pstfx;
    bool unaryMinus = false;
    for (term t : terms)
    {
        switch (t.type)
        {
        case t_const:case t_var: { 
            if(unaryMinus) {
                if(t.name[0] == '-')
                    t.name = t.name.substr(1, t.name.size() - 1);
                else
                    t.name = '-' + t.name;
                
                unaryMinus = false;
            }
            pstfx.push_back(t); break; 
        }
        case t_opbr:s.push(t); break;
        case t_fun:s.push(t); break;
        case t_clbr:
            while (s.top().type != t_opbr)
            {
                pstfx.push_back(s.top());
                s.pop();
            }
            s.pop();
            break;
        case t_op: 
            unaryMinus = (s.empty() || s.top().type == t_opbr) && t.name[0] == '-';
            
            if(!unaryMinus) {
                while (s.size() && s.top().type != t_opbr && (s.top().type == t_fun || priority[s.top().name] >= priority[t.name]))
                {
                    pstfx.push_back(s.top());
                    s.pop();
                }
                s.push(t);
            }
        }
    }
    while (s.size())
    {
        pstfx.push_back(s.top());
        s.pop();
    }
    return pstfx;
}
std::map<string, double(*)(stack<double>& s)> ops;

double pop(stack<double> &s)
{
    double top = s.top();
    s.pop();
    return top;
}

void build_operations_map()
{
    ops["+"] = [](stack<double> &s){ return pop(s) + pop(s); };
    ops["*"] = [](stack<double> &s){ return pop(s) * pop(s); };
    ops["sin"] = [](stack<double> &s){ return sin(pop(s)); };
    ops["cos"] = [](stack<double> &s){ return cos(pop(s)); };
    ops["-"] = [](stack<double> &s){ return -pop(s)+pop(s); };
    ops["/"] = [](stack<double> &s){ double t = pop(s); return t / pop(s);}; 
    ops["tg"] = [](stack<double> &s){ return tan(pop(s)); };
    ops["ctg"] = [](stack<double> &s){ return 1.0/tan(pop(s)); };
    ops["^"] = [](stack<double> &s){ double t = pop(s); return pow(pop(s), t); };
}

double calculate_postfix(vector<term> terms)
{
    if (ops.size() == 0)
        build_operations_map();
    stack<double> s; 
    for (auto elem : terms)
    {
        switch (elem.type)
        {
        case t_const: 
            s.push(stod(elem.name));
            break;
        case t_var:
            //s.push(vars[elem.name]);
            break;
        case t_op:case t_fun:
            s.push(ops[elem.name](s));
        }
    }
    return s.top();
}

pair<string, map<string, double>> get_expressing_from_stream(istream &inp);

// Метод находит лишние скобки в выражении термов
vector<pair<int, int>> FindIncorrectBrackets(const vector<term> &Terms) {
    // Список скобок с их позициями в термах
    vector<pair<int, int>> OpenAndClosedBrackets = vector<pair<int, int>>();
    // Лишние скобки
    vector<pair<int, int>> Result = vector<pair<int, int>>();
    
    // Создаем список скобок и отмечаем позицию открытой скобки и закрытой
    for(int i = 0; i < Terms.size(); i++) {
        // Если встречается открытая скобка, добавляем ее в список
        if(Terms[i].type == t_opbr) {
            OpenAndClosedBrackets.push_back(pair<int, int>(i, -1));
        }
        
        // Если встречается закрытая скобка ставим ее в пару первой свободной скобке, иначе создаем скобку без пары
        if(Terms[i].type == t_clbr) {
            bool Check = false;
            for(int j = OpenAndClosedBrackets.size() - 1; j > -1; j--) {
                if(OpenAndClosedBrackets[j].second == -1) {
                    OpenAndClosedBrackets[j].second = i;
                    Check = true;
                    break;
                }
            }
            
            if(!Check)
                OpenAndClosedBrackets.push_back(pair<int, int>(-1, i));
        }
    }
    
    // Если список содержит одну запись и скобки в начале и конце, возвращаем их позицию
    if(OpenAndClosedBrackets.size() == 1)
        if(OpenAndClosedBrackets[0].first == 0 && OpenAndClosedBrackets[0].second == Terms.size() - 1)
            return OpenAndClosedBrackets;
    
    // Проверяем правильность скобок
    for(int i = 0; i < OpenAndClosedBrackets.size(); i++) {
        // Если внутри скобок пусто или не хватает пары для скобки добавляем в список
        if(OpenAndClosedBrackets[i].first == OpenAndClosedBrackets[i].second - 1 || OpenAndClosedBrackets[i].first == -1 || OpenAndClosedBrackets[i].second == -1) {
            Result.push_back(OpenAndClosedBrackets[i]);
            continue;
        }
        
        // Если скобки вложенны внутрь других скобок без необходимости, например: 1+((1+1))
        if(Terms[OpenAndClosedBrackets[i].first - 1].type == t_opbr && Terms[OpenAndClosedBrackets[i].second + 1].type == t_clbr) {
            Result.push_back(OpenAndClosedBrackets[i]);
            continue;
        }
        
        // Находим минимальный приоритет в скобках
        int MinPrio = -1;
        for(int j = OpenAndClosedBrackets[i].first + 1; j < OpenAndClosedBrackets[i].second; j++) {
            if(Terms[j].type == t_op)
                MinPrio = priority[Terms[j].name] < MinPrio || MinPrio == -1 ? priority[Terms[j].name] : MinPrio;
        }
        
        // Если приоритет внутри скобок выше, чем вне скобок, скобки не нужны
        if(MinPrio != -1) {
            if(OpenAndClosedBrackets[i].first != 0) {
                if(Terms[OpenAndClosedBrackets[i].first - 1].type == t_op)
                    if(priority[Terms[OpenAndClosedBrackets[i].first - 1].name] < MinPrio)
                        Result.push_back(OpenAndClosedBrackets[i]);
            } else if(OpenAndClosedBrackets[i].second != Terms.size() - 1){
                if(Terms[OpenAndClosedBrackets[i].second + 1].type == t_op)
                    if(priority[Terms[OpenAndClosedBrackets[i].second + 1].name] < MinPrio)
                        Result.push_back(OpenAndClosedBrackets[i]);
            }
        }
    }
    
    return Result;
}

// Удалить лишние скобки
vector<term> removeIncorrectBrackets(vector<term> terms, vector<pair<int, int>> incorrectBrackets) {
    for(int i = 0; i < incorrectBrackets.size(); i++) {
		// Удаляем пару скобок из выражения термов
        terms.erase(terms.begin() + incorrectBrackets[i].first);
        terms.erase(terms.begin() + incorrectBrackets[i].second - 1);
        
		// Вычитаем из следующих скобок позиции
        for(int j = i; j < incorrectBrackets.size(); j++) {
            incorrectBrackets[j].first--;
            incorrectBrackets[j].second--;
            
			// Если скобки находятся за предыдущими, то отнимает позицию еще раз
            if(incorrectBrackets[j].first > incorrectBrackets[i].second) {
                incorrectBrackets[j].first--;
                incorrectBrackets[j].second--;
            }
        }
    }
    
    return terms;
}

int main()
{
    string expre = "2+(2*2)-((1+3))-2";
    //std::cout << "abc + 3.15*exp(sin(a - b / c) / (2.0 + tg(x^y)) - x / y*z)*abc\n";
    /*for (auto elem : ya_get_terms_from_expression("abc+3.15*exp(sin(a-b/c)/(2.0+tg(x^y))-x/y*z)*abc"))
    {
    cout << elem.name << " " << elem.type << std::endl;
    }
    cout << endl;*/
    vector<term> terms = ya_get_terms_from_expression(expre);
    vector<pair<int, int>> OpAndClBrackets = FindIncorrectBrackets(terms);
    
    for(pair<int, int> i : OpAndClBrackets)
        cout << "Pair: " << i.first << " " << i.second << endl;
    
    vector<term> correctTerms = removeIncorrectBrackets(terms, OpAndClBrackets);
    
    cout << endl;
    for(term i : correctTerms)
        cout << i.name << " type: " << i.type << endl;
    //vector<term> terms = get_postfix_from_terms(ya_get_terms_from_expression(expre));
    //for (auto elem : terms)
    //    cout << elem.name << " ";
    
    //cout << endl << endl;
    //cout << calculate_postfix(terms);
    
    cout << endl << "RESULT: " << calculate_postfix(get_postfix_from_terms(ya_get_terms_from_expression("1*3/0.1-1")));
    return 0;
}
