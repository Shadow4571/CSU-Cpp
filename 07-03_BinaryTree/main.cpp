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

template<typename K, typename V>
class binary_tree {
    private:
    // Класс ноды дерева
    class tree_node {
        private:
        K Key;              // Ключ ноды по которому строится дерево
        V Value;            // Значение в ноде по указанному ключу
        tree_node *Left;    // Указатель на левую ноду
        tree_node *Right;   // Указатель на правую ноду
            
        public:
        // Конструктор принимает ключ и значение для создание корневой ноды
        tree_node(K Key, V Value) {
            this->Key = Key;
            this->Value = Value;
            this->Left = nullptr;
            this->Right = nullptr;
        }
        
        K getKey() { return this->Key; }
        V getValue() { return this->Value; }
        
        // Устанавливаем ноду слева
        bool setLeft(tree_node &Left) {
            if(this->Left != nullptr)
                return false;
            
            this->Left = &Left;
            return true;
        }
        
        // Устанавливаем ноду справа
        bool setRight(tree_node &Right) {
            if(this->Right != nullptr)
                return false;
            
            this->Right = &Right;
            return true;
        }
        
        tree_node* getLeft() { return this->Left; }
        tree_node* getRight() { return this->Right; }
        
        bool operator==(const tree_node &comp) {
            return this->Key == comp.getKey();
        }
    };
    
    tree_node *Root;    // Корневая нода дерева
    int Count;          // Количество нод в дереве
    
    public:
    binary_tree(K Key, V Value) {
        this->Root = new tree_node(Key, Value);
        this->Count = 1;
    }
    
    int getCount() { return this->Count; }
    
    // Содержится ли ключ в дереве
    bool contains(const K &Key) {
        tree_node* Temp = this->Root;
        
        while(Temp != nullptr) {
            K TempKey = Temp->getKey();
            
            if(TempKey == Key)
                return true;
            
            if(Key < TempKey)
                Temp = Temp->getLeft();
            else
                Temp = Temp->getRight();
        }
        
        return false;
    }
    
    // Содержится ли значение в дереве
    bool contains(const V &Value) {
        vector<V> Values = this->getAllValues();
        
        for(V i : Values)
            if(i == Value)
                return true;
        
        return false;
    }
    
    // Добавить ноду в дерево
    bool add(const K &Key, const V &Value) {
        if(this->contains(Key))
            return false;
        
        tree_node *Temp = this->Root;
        
        while(true) {
            if(Key < Temp->getKey()) {
                if(Temp->getLeft() != nullptr) {
                    Temp = Temp->getLeft();
                } else {
                    Temp->setLeft(*(new tree_node(Key, Value)));
                    this->Count++;
                    return true;
                }
            } else {
                if(Temp->getRight() != nullptr) {
                    Temp = Temp->getRight();
                } else {
                    Temp->setRight(*(new tree_node(Key, Value)));
                    this->Count++;
                    return true;
                }
            }
        }
    }
    
    // Получить значение по ключу
    V find(const K &Key) {
        if(!this->contains(Key))
            throw "Cannot find element by key";
        
        tree_node *Temp = this->Root;
        
        while(Key != Temp->getKey()) {
            if(Key < Temp->getKey())
                Temp = Temp->getLeft();
            else
                Temp = Temp->getRight();
        }
        
        return Temp->getValue();
    }
    
    // Получить все значения из дерева
    vector<V> getAllValues(vector<V> Values = vector<V>(), tree_node *Node = nullptr) {
        Node = Node == nullptr ? this->Root : Node;
        
        Values.push_back(Node->getValue());
        
        if(Node->getLeft() != nullptr)
            Values = this->getAllValues(Values, Node->getLeft());
        
        if(Node->getRight() != nullptr)
            Values = this->getAllValues(Values, Node->getRight());
        
        return Values;
    }
};

int main() {
    binary_tree<int, string> Tree = binary_tree<int, string>(8, "Hello");
    Tree.add(6, "World");
    Tree.add(10, "!");
    
    vector<string> Words = Tree.getAllValues();
    for(string i : Words)
        cout << i << endl;
    
    string Hello, World, Exclamation;
    
    try {
        Hello = Tree.find(8);
        World = Tree.find(6);
        Exclamation = Tree.find(10);
    } catch(const char* Exp) {
        cout << Exp;
    }
    
    cout << endl << Hello << " " << World << Exclamation << endl;
    
    cout << endl << boolalpha << Tree.contains("Hello") << " " << Tree.contains("Privet") << endl;
    
    return 0;
}
