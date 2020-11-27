#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <stdlib.h>
#include <cstring>
#include <utility>
#include <iomanip>

using namespace std;

template<typename K, typename V>
class binary_tree {
    public:
	// Перечисление для выбора типа сортировки (Не сортировать, от меньшего к большему, от большего к меньшему)
    enum sortedType {ST_nonSort, ST_minMax, ST_maxMin};
    
    private:
    // Класс ноды дерева
    class tree_node {
        private:
        K Key;              // Ключ ноды по которому строится дерево
        V Value;            // Значение в ноде по указанному ключу
        tree_node *Left;    // Указатель на левую ноду
        tree_node *Right;   // Указатель на правую ноду
        tree_node *Parent;  // Указатель на родителя
            
        public:
        // Конструктор принимает ключ и значение для создание корневой ноды
        tree_node(K Key, V Value, tree_node* Parent = nullptr) {
            this->Key = Key;
            this->Value = Value;
            this->Left = nullptr;
            this->Right = nullptr;
            this->Parent = Parent;
        }
        
        K getKey() { return this->Key; }
        V getValue() { return this->Value; }
        
        // Устанавливаем ноду слева
        void setLeft(tree_node &Left) { this->Left = &Left; }
        
        // Устанавливаем ноду справа
        void setRight(tree_node &Right) { this->Right = &Right; }
        
        void setLeft(tree_node* Left) { this->Left = Left; }
        void setRight(tree_node* Right) { this->Right = Right; }
        void setParent(tree_node* Parent) { this->Parent = Parent; }
        
        tree_node* getLeft() { return this->Left; }
        tree_node* getRight() { return this->Right; }
        tree_node* getParent() { return this->Parent; }
        
        bool operator==(const tree_node &comp) {
            return this->Key == comp.getKey();
        }
        
        string toString() {
            return "Node with Key: " + to_string(this->Key) + " " + this->Value; 
        }
    };
    
    // Структура визитера дерева
    struct tree_visitor {
        tree_node *Current;		// Указатель на текущую ноду
        vector<K> Visited;		// Вектор ключей, которые визитер посетил
        int Count;				// Количество нод в дереве
        
        tree_visitor(tree_node* Start) {
            Current = Start;
            Visited = vector<K>();
            Count = 1;
        }
        
		// Проверяет была ли посещена вершина с данным ключом
        bool isVisit(K Key) { return find(Visited.begin(), Visited.end(), Key) != Visited.end(); }
        
		// Возвращает текущую ноду дерева
        tree_node* get() { 
            if(Current == nullptr)
                throw string("Visitor step over all nodes\n");
            
            if(isVisit(Current->getKey()))
                next();
            
            return Current;
        }
        
		// Сбросить визитера к начальное состояние
        void reset() {
            Current = Root;
            Visited.clear();
        }
        
		// Проверить существуют ли еще непосещенные ноды
        bool hasNext() { return Visited.size() < Count; }
		// Перейти к следующей непосещенной ноде
        void next() {
            if(!hasNext())
                throw string("Do not find next element\n");
            
			// Помечаем текущую ноду как посещенную
            if(!isVisit(Current->getKey()))
                Visited.push_back(Current->getKey());
            
			// Если левая нода не была посещенна, заходим в нее
            if(Current->getLeft() != nullptr) {
                if(!isVisit(Current->getLeft()->getKey())) {
                    Current = Current->getLeft();
                    return;
                }
            }
            
			// Если правая нода не была посещена, заходим в нее
            if(Current->getRight() != nullptr) {
                if(!isVisit(Current->getRight()->getKey())) {
                    Current = Current->getRight();
                    return;
                }
            }
            
			// Если левая и правая нода посещены, возвращаемся до первой непосещенной ноды
            while(Current != nullptr) {
                if(Current->getLeft() != nullptr)
                    if(!isVisit(Current->getLeft()->getKey()))
                        break;
                    
                if(Current->getRight() != nullptr)
                    if(!isVisit(Current->getRight()->getKey()))
                        break;
                    
                Current = Current->getParent();
            }
        }
    };
    
    // Нода для вывода дерева в косоль
    struct node_print_state_t {
        node_print_state_t* child_state;
        bool printing_last_child;
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
    
	// Найти ноду по ключу
    tree_node* findNode(K Key) {
        tree_node* Node = this->Root;
        
        while(Node->getKey() != Key) {
            if(Key > Node->getKey())
                Node = Node->getRight();
            else
                Node = Node->getLeft();
        }
        
        return Node;
    }
    
    node_print_state_t* _root_state;
    
	// Метод выводит дерево в консоль
    void finePrintTree(tree_node* node) {
        node_print_state_t* parent_state;
        if (_root_state != nullptr) {
            cout << ' ';
            node_print_state_t* s = _root_state;
            while (s->child_state != nullptr) {
                printf(s->printing_last_child ? "  " : "| ");
                s = s->child_state;
            }
            parent_state = s;
            printf(parent_state->printing_last_child ? "L" : "+");
        } else {
            parent_state = nullptr;
        }
        cout << '>' << node->getValue() << '\n';
        
        if ((node->getLeft() != nullptr) || (node->getRight() != nullptr)) { 
            node_print_state_t s;
            if (parent_state != nullptr) {
                parent_state->child_state = &s;
            } else {
                _root_state = &s;
            }
            s.child_state = nullptr;
            
            if (node->getLeft() != nullptr) {
                s.printing_last_child = (node->getRight() == nullptr);
                finePrintTree(node->getLeft());
            }
            if (node->getRight() != nullptr) {
                s.printing_last_child = true;
                finePrintTree(node->getRight());
            }
            
            if (parent_state != nullptr) {
                parent_state->child_state = nullptr;
            } else {
                _root_state = nullptr;
            }
        }
    }
    
	// Посчитать ширину в правых нодах
    vector<int> countSubRightNodes(tree_node *Node, int Width, vector<int> Result = vector<int>()) {        
        while(Node != nullptr) {
            if(Node != nullptr) {
                if(Node->getRight() != nullptr) {
                    vector<int> Temp = this->countSubRightNodes(Node->getRight(), Width - 1, Result);
                    Result.insert(Result.end(), Temp.begin(), Temp.end());
                }
                
                if(Node->getLeft() != nullptr)
                   Width++;
                
                Node = Node->getLeft();
            }
        }
        
        Result.push_back(Width);
        return Result;
    }
    
	// Посчитать ширину в левых нодах
    vector<int> countSubLeftNodes(tree_node *Node, int Width, vector<int> Result = vector<int>()) {
        while(Node != nullptr) {
            if(Node != nullptr) {
                if(Node->getLeft() != nullptr) {
                    vector<int> Temp = this->countSubLeftNodes(Node->getLeft(), Width - 1, Result);
                    Result.insert(Result.end(), Temp.begin(), Temp.end());
                }
                
                if(Node->getRight() != nullptr)
                   Width++;
                
                Node = Node->getRight();
            }
        }
        
        Result.push_back(Width);
        return Result;
    }
    
    // Получить все значения из дерева
    vector<V> getAllValues(vector<V> Values, tree_node *Node) {
        Node = Node == nullptr ? this->Root : Node;
        
        if(Node->getLeft() != nullptr)
            Values = this->getAllValues(Values, Node->getLeft());
        
        Values.push_back(Node->getValue());
        
        if(Node->getRight() != nullptr)
            Values = this->getAllValues(Values, Node->getRight());
        
        return Values;
    }
    
	// Вернуть вектор отсортированных пар ключ/значений
    vector<pair<K, V>> getAllSortedNodes(tree_node *Node, sortedType Type) {
        vector<pair<K, V>> Nodes = this->getAllNodesDFS(Node);
        if(Type == ST_minMax)
            sort(Nodes.begin(), Nodes.end(), [](pair<K, V> a, pair<K, V> b) { return a.first < b.first; });
        
        if(Type == ST_maxMin)
            sort(Nodes.begin(), Nodes.end(), [](pair<K, V> a, pair<K, V> b) { return a.first > b.first; });
        
        return Nodes;
    }
    
    vector<pair<K, V>> getAllNodesDFS(tree_node *Node, vector<pair<K, V>> Nodes = vector<pair<K, V>>()) {
        if(Node->getLeft() != nullptr)
            Nodes = this->getAllNodesDFS(Node->getLeft(), Nodes);
        
        Nodes.push_back(pair<K, V>(Node->getKey(), Node->getValue()));
        
        if(Node->getRight() != nullptr)
            Nodes = this->getAllNodesDFS(Node->getRight(), Nodes);
        
        return Nodes;
    }
    
    int getHeight(tree_node* Temp, int Height = 0) {
        if(Temp == nullptr)
            return Height;
        
        int Left = getHeight(Temp->getLeft(), Height + 1);
        int Right = getHeight(Temp->getRight(), Height + 1);
        
        return max(Left, Right);
    }
    
    tree_node *Root;    // Корневая нода дерева
    tree_visitor visitor = NULL;
    
    /* PUBLIC METHODS */
    public:
    binary_tree(K Key, V Value) {
        this->Root = new tree_node(Key, Value);
        _root_state = nullptr;
        this->visitor = tree_visitor(this->Root);
    }
    
    // Публичные методы для работы с визитером
    pair<K, V> visitorGet() { return pair<K, V>(this->visitor.get()->getKey(), this->visitor.get()->getValue()); }
    void visitorNext() { this->visitor.next(); }
    bool visitorHasNext() { return this->visitor.hasNext(); }
    void visitorReset() { this->visitor.reset(); }
    
    // Вернуть количество нод и проверить пустое ли дерево
    int getCount() { return this->visitor.Count; }
    bool isEmpty() { return this->Root == nullptr; }
    
    // Содержится ли ключ в дереве
    bool contains(const K &Key) {
        if(this->isEmpty())
            return false;
        
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
        if(this->isEmpty())
            return false;
        
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
        
        this->visitor.Count++;
        if(this->isEmpty()) {
            this->Root = new tree_node(Key, Value);
            return true;
        }
        
        tree_node *Temp = this->Root;
        
        while(true) {
            if(Key < Temp->getKey()) {
                if(Temp->getLeft() != nullptr) {
                    Temp = Temp->getLeft();
                } else {
                    Temp->setLeft(*(new tree_node(Key, Value, Temp)));
                    return true;
                }
            } else {
                if(Temp->getRight() != nullptr) {
                    Temp = Temp->getRight();
                } else {
                    Temp->setRight(*(new tree_node(Key, Value, Temp)));
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
    
	// Обновленный метод удаления ноды дерева
    bool erase(const K &Key) {
		// Если ключа нету в дереве, нечего удалять
        if(!this->contains(Key))
            return false;
        
        tree_node *Temp = this->findNode(Key);
        this->visitor.Count--;
        
        if(Temp == this->Root) {
            if(Temp->getLeft() == nullptr && Temp->getRight() == nullptr) {
                this->Root = nullptr;
                return true;
            }
            
            tree_node* toSet = this->Root->getLeft() != nullptr ? this->Root->getLeft() : this->Root->getRight();
            tree_node* child = this->Root->getLeft() != nullptr ? this->Root->getRight() : nullptr;
            
            this->Root = toSet;
            this->Root->setParent(nullptr);
            
            if(child != nullptr) {
                while(toSet->getRight() != nullptr)
                    toSet = toSet->getRight();
                
                toSet->setRight(child);
            }
        } else {
            tree_node* Parent = Temp->getParent();
            bool isLeft = Temp->getKey() < Parent->getKey();
            
            if(Temp->getLeft() == nullptr && Temp->getRight() == nullptr) {
                if(isLeft)
                    Parent->setLeft(nullptr);
                else
                    Parent->setRight(nullptr);
                
                return true;
            }
            
            tree_node* toSet = Temp->getLeft() != nullptr ? Temp->getLeft() : Temp->getRight();
            tree_node* child = Temp->getLeft() != nullptr ? Temp->getRight() : nullptr;
            
            if(isLeft)
                Parent->setLeft(toSet);
            else
                Parent->setRight(toSet);
            
            if(child != nullptr) {
                while(toSet->getRight() != nullptr)
                    toSet = toSet->getRight();
                
                toSet->setRight(child);
            }
        }
        
        return true;
    }
    
	// Вывести дерево в консоль
    void printTree() {
        this->finePrintTree(this->Root);
    }
    
	// Вернуть ширину дерева
    int getTreeWidth() {
        if(this->Root->getLeft() == nullptr && this->Root->getRight() == nullptr)
            return 0;
        
        tree_node *Left = this->Root->getLeft(), *Right = this->Root->getRight();
        int LeftWidth = Left != nullptr ? 1 : 0, RightWidth = Right != nullptr ? 1 : 0;
        vector<int> LeftResult = vector<int>();
        vector<int> RightResult = vector<int>();
        
        while(Left != nullptr || Right != nullptr) {
            if(Left != nullptr) {
                if(Left->getRight() != nullptr) {
                    vector<int> LeftTemp = this->countSubRightNodes(Left->getRight(), LeftWidth - 1);
                    LeftResult.insert(LeftResult.end(), LeftTemp.begin(), LeftTemp.end());
                }
                
                LeftWidth = Left->getLeft() != nullptr ? LeftWidth + 1 : LeftWidth;
                Left = Left->getLeft();
            }
            
            if(Right != nullptr) {
                if(Right->getLeft() != nullptr) {
                    vector<int> RightTemp = this->countSubLeftNodes(Right->getLeft(), RightWidth - 1);
                    RightResult.insert(RightResult.end(), RightTemp.begin(), RightTemp.end());
                }
                
                RightWidth = Right->getRight() != nullptr ? RightWidth + 1 : RightWidth;
                Right = Right->getRight();
            }
        }
        
        LeftResult.push_back(LeftWidth); RightResult.push_back(RightWidth);
        
        int LeftMax = LeftResult[distance(LeftResult.begin(), max_element(LeftResult.begin(), LeftResult.end()))];
        int RightMax = RightResult[distance(RightResult.begin(), max_element(RightResult.begin(), RightResult.end()))];
        return LeftMax + RightMax - 1;
    }
    
	// Вернуть высоту дерева
    int getTreeHeight() {
        return this->getHeight(this->Root);
    }
    
	// Вернуть ноду перед нодой по заданному ключу
    pair<K, V> getPrevValue(K Key) {
        if(!this->contains(Key))
            throw "Tree does not contains key";
        
		tree_node* Temp = this->findPrevNode(Key);
        return pair<K, V>(Temp->getKey(), Temp->getValue());
    }
    
	// Вернуть правую или левую ноду следующую после заданного ключа
    pair<K, V> getNextValue(K Key, bool isLeft) {
        if(!this->contains(Key))
            throw "Tree does not contains key";
        
        tree_node* Temp = this->findNode(Key);
        
        if(isLeft && Temp->getLeft() != nullptr)
            return pair<K, V>(Temp->getLeft()->getKey(), Temp->getLeft()->getValue());
        
        if(!isLeft && Temp->getRight() != nullptr)
            return pair<K, V>(Temp->getRight()->getKey(), Temp->getRight()->getValue());
        
        string Message = "Node does not contains next ";
        Message += isLeft ? "left" : "right"; Message += " node";
        throw Message;
    }
    
	// Вернуть все значения из дерева
    vector<V> getAllValues() {
        return this->getAllValues(vector<V>(), this->Root);
    }
    
	// Вернуть все пары ключ/значений
    vector<pair<K, V>> getAllNodes(sortedType Type) {
        switch(Type) {
            case ST_nonSort:
                return this->getAllNodesDFS(this->Root);
                
            case ST_minMax:
                return this->getAllSortedNodes(this->Root, Type);
                
            default:
                return this->getAllNodesDFS(this->Root);
        }
    }
    
	// Отсортировать все ноды по ключу и выполнить для каждой внешнюю функцию
    typedef void(*FunctionPtr)(pair<K, V>);
    void executeFunctionForSortedNodes(const FunctionPtr& functionPtr) {
        vector<pair<K, V>> sortedNodes = this->getAllNodes(ST_minMax);
        
        for(pair<K, V> i : sortedNodes)
            functionPtr(i);
    }
};

template<typename V>
void printTreeValues(vector<V> allValues) {
    cout << endl << "Values:" << endl;
    for(V i : allValues)
        cout << i << " ";
    cout << endl;
}

void printNode(pair<int, string> Node) {
    cout << "Key: " << Node.first << " Value: " << Node.second << endl;
}

int main() {
    binary_tree<int, string> numbersTree = binary_tree<int, string>(10, "10");
    numbersTree.add(5, "5"); numbersTree.add(8, "8"); numbersTree.add(7, "7");
    numbersTree.add(4, "4"); numbersTree.add(-5, "-5"); numbersTree.add(3, "3");
    numbersTree.add(2, "2"); numbersTree.add(1, "1"), numbersTree.add(0, "0");
    numbersTree.add(-1, "-1");
    numbersTree.add(11, "11"); numbersTree.add(12, "12");
    
    numbersTree.printTree();
    int Width = numbersTree.getTreeWidth();
    
    cout << endl << "Width: " << Width << endl << "Height: " << numbersTree.getTreeHeight() << endl << endl;
    
    numbersTree.executeFunctionForSortedNodes(&printNode);
    
    cout << endl << "SIZE: " << numbersTree.getCount() << endl;
    try {
        while(numbersTree.visitorHasNext()) {
            pair<int, string> temp = numbersTree.visitorGet();
            numbersTree.visitorNext();
            
            cout << "Key: " << temp.first << " Value: " << temp.second << endl;
        }
    } catch(string Exp) {
        cout << endl << Exp << endl;
    }
    
    numbersTree.erase(5);
    numbersTree.erase(10);
    
    vector<pair<int, string>> allNodes = numbersTree.getAllNodes(numbersTree.ST_nonSort);
	
	cout << endl;
	for(pair<int, string> i : allNodes)
		cout << "Key: " << i.first << " Value: " << i.second << endl;
    
    return 0;
}
