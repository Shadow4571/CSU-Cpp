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
        void setLeft(tree_node &Left) { this->Left = &Left; }
        
        // Устанавливаем ноду справа
        void setRight(tree_node &Right) { this->Right = &Right; }
        
        void setLeft(tree_node* Left) { this->Left = Left; }
        void setRight(tree_node* Right) { this->Right = Right; }
        
        tree_node* getLeft() { return this->Left; }
        tree_node* getRight() { return this->Right; }
        
        bool operator==(const tree_node &comp) {
            return this->Key == comp.getKey();
        }
        
        string toString() {
            return "Node with Key: " + to_string(this->Key) + " " + this->Value; 
        }
    };
    
	// Найти ноду перед удаляемой
    tree_node* findPrevNode(K Key) {
        tree_node *Temp = this->Root;
        
        if(Temp->getKey() == Key)
            return Temp;
        
        while(true) {
            if(Key < Temp->getKey()) {
                if(Temp->getLeft()->getKey() == Key)
                    return Temp;
                
                Temp = Temp->getLeft();
            } else {
                if(Temp->getRight()->getKey() == Key)
                    return Temp;
                
                Temp = Temp->getRight();
            }
        }
    }
    
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
    V get(const K &Key) {
        if(!this->contains(Key))
            throw "Cannot get element by key";
        
        tree_node *Temp = this->Root;
        
        while(Key != Temp->getKey()) {
            if(Key < Temp->getKey())
                Temp = Temp->getLeft();
            else
                Temp = Temp->getRight();
        }
        
        return Temp->getValue();
    }
    
	// Удалить ноду по ключу
    bool erase(const K &Key) {
		// Если ключа нету в дереве, нечего удалять
        if(!this->contains(Key))
            return false;
        
		// Получает ноду перед той, которую нужно удалить
        tree_node *Temp = this->findPrevNode(Key);
        
		// Если эта нода корневая и содержит ключ, удаляем корневую ноду
        if(Key == this->Root->getKey()) {
            tree_node* TempLeft = Temp->getLeft();
            Temp = Temp->getRight();
            delete this->Root;
            this->Root = Temp;
            
            while(Temp->getLeft() != nullptr)
                Temp = Temp->getLeft();
            
            Temp->setLeft(TempLeft);
        } else {
			// Проверяем находится ли удаляемая нода слева, иначе справа
            bool isLeft = Temp->getLeft() != nullptr ? Temp->getLeft()->getKey() == Key : false;
			// Получаем левую и правую ноду, после удаляемой
            tree_node* TempLeft = isLeft ? Temp->getLeft()->getLeft() : Temp->getRight()->getLeft();
            tree_node* TempRight = isLeft ? Temp->getLeft()->getRight() : Temp->getRight()->getRight();
            
			// Если у удаляемой ноды нету связей, удаляем ноду
            if(TempLeft == nullptr && TempRight == nullptr) {
                if(isLeft)
                    Temp->setLeft(nullptr);
                else
                    Temp->setRight(nullptr);
                
                return true;
            }
            
			// Если у удаляемой ноды есть одна связь справа, устанавливаем ее
            if(TempLeft == nullptr) {
                if(isLeft)
                    Temp->setLeft(TempRight);
                else
                    Temp->setRight(TempRight);
            
                return true;
            }
            
			// Если у удаляемой ноды есть одна связь слева, устанавливаем ее
            if(TempRight == nullptr) {
                if(isLeft)
                    Temp->setLeft(TempLeft);
                else
                    Temp->setRight(TempLeft);
                
                return true;
            }
            
			// Если есть левая и правая связь
            if(isLeft)
                Temp->setLeft(TempRight);
            else
                Temp->setRight(TempRight);
            
            while(TempRight->getLeft() != nullptr)
                TempRight = TempRight->getLeft();
            
            TempRight->setLeft(TempLeft);
        }
        
        return true;
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

template<typename V>
void printTreeValues(vector<V> allValues) {
    for(V i : allValues)
        cout << i << endl;
}

int main() {
    binary_tree<int, string> Tree = binary_tree<int, string>(10, "10");
    Tree.add(5, "5"); Tree.add(15, "15");
    Tree.add(1, "1"); Tree.add(8, "8");
    Tree.add(12, "12"); Tree.add(17, "17");
    Tree.add(13, "13"); Tree.add(9, "9");
    
    printTreeValues<string>(Tree.getAllValues());
    Tree.erase(10);
    Tree.erase(17);
    cout << endl;
    printTreeValues<string>(Tree.getAllValues());
    cout << endl;
    
    binary_tree<string, int> numbersTree = binary_tree<string, int>("one", 1);
    numbersTree.add("two", 2); numbersTree.add("three", 3);
    numbersTree.add("four", 4); numbersTree.add("five", 5);
    numbersTree.add("six", 6); numbersTree.add("seven", 7);
    
    printTreeValues<int>(numbersTree.getAllValues());
    
    return 0;
}
