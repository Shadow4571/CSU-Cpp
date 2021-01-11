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

vector<pair<int, vector<int>>> LEAF_SET = vector<pair<int, vector<int>>> {   
    pair<int, vector<int>>{0, vector<int>{1, 2, 3}},
        pair<int, vector<int>>{1, vector<int>{0, 4, 5}}, 
        pair<int, vector<int>>{2, vector<int>{0, 9}},
        pair<int, vector<int>>{3, vector<int>{0}},
        pair<int, vector<int>>{4, vector<int>{1, 6}},
        pair<int, vector<int>>{5, vector<int>{1, 7, 8}},
        pair<int, vector<int>>{6, vector<int>{4}},
        pair<int, vector<int>>{7, vector<int>{5}},
        pair<int, vector<int>>{8, vector<int>{5}},
        pair<int, vector<int>>{9, vector<int>{2}}
};

struct leaf {
    int _pos;
    vector<leaf> _bonds;
    
    leaf(int position) {
        _pos = position;
        _bonds = vector<leaf>();
    }
    
    bool operator==(const leaf &a) {
        return this->_pos == a._pos;
    }
    
    bool operator==(leaf &a) {
        return this->_pos == a._pos;
    }
};

vector<leaf> CreateTree(vector<pair<int, vector<int>>> &LeafSet) {
    vector<leaf> Result;
    
    for(int i = 0; i < LeafSet.size(); i++)
        Result.push_back(leaf(LeafSet[i].first));
    
    for(int i = 0; i < LeafSet.size(); i++) {
        Result[i]._pos = LeafSet[i].first;
        for(int j = 0; j < LeafSet[i].second.size(); j++) {
            Result[i]._bonds.push_back(Result[LeafSet[i].second[j]]);
        }
    }
    
    return Result;
}

void PrintTree(vector<leaf> &Tree) {
    cout << "==== TREE ====" << endl;
    for(leaf i : Tree) {
        cout << "Leaf " << i._pos << endl;
        for(leaf j : i._bonds) {
            cout << "   bond: " << j._pos << endl;
        }
    }
    cout << "==============" << endl;
}

vector<leaf>::iterator GetMinLeaf(vector<leaf> &Tree) {
    vector<leaf>::iterator Result = Tree.begin();
    
    for(vector<leaf>::iterator i = Tree.begin()++; i != Tree.end(); i++)
        if(i->_bonds.size() < Result->_bonds.size())
        Result = i;
    
    return Result;
}

vector<int> GetPruferCode(vector<leaf> &Tree) {
    vector<int> Result;
    
    int Count = 0;
    while(Tree.size() > 2) {
        vector<leaf>::iterator Min = GetMinLeaf(Tree);
        vector<leaf>::iterator Bond = find(Tree.begin(), Tree.end(), Min->_bonds[0]);
        
        Result.push_back(Bond->_pos);
        
        Bond->_bonds.erase(find(Bond->_bonds.begin(), Bond->_bonds.end(), *Min));
        Tree.erase(Min);
    }
    
    return Result;
}

vector<int> GetNorepeat(vector<int> &f, vector<int> &s) {
    int size = f.size() > s.size() ? f.size() : s.size();
    vector<int> result = vector<int>();
    for(int i = 0; i < size; i++) {
        if(i < f.size()) {
            if(find(s.begin(), s.end(), f[i]) == s.end())
                result.push_back(f[i]);
        } else if(i < s.size()) {
            if(find(f.begin(), f.end(), s[i]) == f.end())
                result.push_back(s[i]);
        }
    }
    
    sort(result.begin(), result.end());
    return result;
}

vector<int> GetGraphVertexes(vector<leaf> &Tree) {
    vector<int> result = vector<int>();
    for(leaf i : Tree)
        result.push_back(i._pos);
    
    return result;
}

int FindMinInPrufer(vector<int> &Prufer, vector<int> &Vertexes) {
    return GetNorepeat(Prufer, Vertexes)[0];
}

void AddToTree(vector<leaf> *Tree, vector<leaf>::iterator tempLeaf, int vertex, int code) {
    if(tempLeaf == Tree->end()) {
        Tree->push_back(leaf(vertex));
        Tree->at(Tree->size() - 1)._bonds.push_back(leaf(code));
    } else {
        if(find(tempLeaf->_bonds.begin(), tempLeaf->_bonds.end(), code) == tempLeaf->_bonds.end())
            Tree->at(distance(Tree->begin(), tempLeaf))._bonds.push_back(leaf(code));
    }
}

vector<leaf> RollbackPruferCode(vector<leaf> Tree, vector<int> &PruferCode, vector<int> &Vertexes) {
    while(PruferCode.size() != 0) {
        int code = PruferCode[0];
        int vertex = FindMinInPrufer(Vertexes, PruferCode);
        
        PruferCode.erase(find(PruferCode.begin(), PruferCode.end(), code));
        Vertexes.erase(find(Vertexes.begin(), Vertexes.end(), vertex));
        
        vector<leaf>::iterator tempLeaf = find(Tree.begin(), Tree.end(), leaf(vertex));
        AddToTree(&Tree, tempLeaf, vertex, code);
        
        vector<leaf>::iterator tempCode = find(Tree.begin(), Tree.end(), leaf(code));
        AddToTree(&Tree, tempCode, code, vertex);
    }
    
    return Tree;
}

bool checkBipatrition(vector<leaf>::iterator vertex, vector<leaf> &vertexes, bool color, vector<pair<int, bool>> *checkedVertexes) {
    if(vertex == vertexes.end())
        return true;
    
    int i = 0;
    while(checkedVertexes->at(i).first != vertex->_pos || i != checkedVertexes->size())
        i++;
    
    if(i == checkedVertexes->size())
        checkedVertexes->push_back(pair<int, double>(vertex->_pos, color));
    else if(checkedVertexes->at(i).second != color)
        return false;
    
    vector<bool> result = vector<bool>();
    vector<leaf>::iterator bond = vertex->_bonds.begin();
    
    while(bond != vertex->bonds.end()) {
        result.push_back(checkBipatrition(find(vertexes.begin(), vertexes.end(), leaf(bond->_pos)), vertexes, !color, checkedVertexes));
        bond++;
    }
    
    return find(result.begin(), result.end(), false) != result.end;
}

int main() {
    vector<leaf> Tree = CreateTree(LEAF_SET);
    
    cout << "INPUT TREE" << endl;
    PrintTree(Tree);
    cout << endl;
    
    vector<int> GraphVertexes = GetGraphVertexes(Tree);
    vector<int> PruferCode = GetPruferCode(Tree);
    
    cout << endl << "TREE AFTER PRUFER" << endl;
    PrintTree(Tree);
    
    cout << endl << endl << "Prufer Code:" << endl;
    for(int i : PruferCode)
        cout << i << " ";
    cout << endl << endl;
    
    vector<leaf> NewTree = RollbackPruferCode(Tree, PruferCode, GraphVertexes);
    
    cout << endl << "TREE AFTER ROLLBACK" << endl;
    PrintTree(NewTree);
    
    return 0;
}
