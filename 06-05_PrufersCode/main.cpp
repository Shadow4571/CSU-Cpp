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
    }
    
    const bool operator==(const leaf &t) const { return _pos == t._pos; }
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

int main() {
    vector<leaf> Tree = CreateTree(LEAF_SET);
    PrintTree(Tree);
    vector<int> PruferCode = GetPruferCode(Tree);
    
    cout << endl << "Prufer Code:" << endl;
    for(int i : PruferCode)
        cout << i << " ";
    
    return 0;
}
