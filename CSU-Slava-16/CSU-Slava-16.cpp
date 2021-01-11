#include <filesystem>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

template <typename K, typename V>
class BST {
public:
	/* BST_Entry - класс вхождения дерева, содержит ключ и значение. */
	class BST_Entry {
	private:
		K key;
		V value;

	public:
		BST::BST_Entry() {}

		BST::BST_Entry(K key, V value) {
			this->key = key;
			this->value = value;
		}

		K getKey() { return this->key; }
		V getValue() { return this->value; }
	};

	/* BST_Node - класс ноды дерева, содержит вхождение и указатели на левую, правую и родительскую ноду. */
	class BST_Node {
	protected:
		BST::BST_Entry entry;
		BST::BST_Node* left;
		BST::BST_Node* right;
		BST::BST_Node* parent;

	public:
		BST::BST_Node(K key, V value, BST::BST_Node* parent = nullptr, BST::BST_Node* left = nullptr, BST::BST_Node* right = nullptr) : BST::BST_Node(BST::BST_Entry(key, value), parent, left, right) {}

		BST::BST_Node(BST::BST_Node* from, BST::BST_Node* parent = nullptr, BST::BST_Node* left = nullptr, BST::BST_Node* right = nullptr) : BST::BST_Node(from->getEntry(), parent, left, right) {}

		BST::BST_Node(BST::BST_Entry entry, BST::BST_Node* parent = nullptr, BST::BST_Node* left = nullptr, BST::BST_Node* right = nullptr) {
			this->entry = entry;
			this->left = left;
			this->right = right;
			this->parent = parent;
		}


		BST::BST_Entry getEntry() { return this->entry; }

		K getKey() { return this->entry.getKey(); }
		V getValue() { return this->entry.getValue(); }

		BST::BST_Node* getLeft() { return this->left; }
		BST::BST_Node* getRight() { return this->right; }
		BST::BST_Node* getParent() { return this->parent; }

		void setLeft(BST::BST_Node* left) { this->left = left; }
		void setRight(BST::BST_Node* right) { this->right = right; }
		void setParent(BST::BST_Node* parent) { this->parent = parent; }

		bool equals(K key) { return this->entry.getKey() == key; }
		bool equals(BST::BST_Node node) { return this->getKey() == node.getKey(); }
	};

private:
	BST::BST_Node* root = nullptr;
	unsigned long long int size = 0;

	/* BST_Visitor - структура визитера дерева. Обходит по порядку все ноды дерева. */
	struct BST_Visitor {
		BST::BST_Node* current;
		std::vector<K> visited;
		unsigned long long int count;

		BST::BST_Visitor() {
			current = nullptr;
			visited = std::vector<K>();
			count = 0;
		}

		BST::BST_Visitor(BST::BST_Node* Start, unsigned long long int size) {
			current = Start;
			visited = std::vector<K>();
			count = size;
		}

		void addCount() {
			this->count++; 
		}

		void removeCount() {
			this->count--;
			this->current = this->count == 0 ? nullptr : this->current;
		}

		bool isVisit(K key) { return find(visited.begin(), visited.end(), key) != visited.end(); }

		BST::BST_Node* get() {
			if (current == nullptr)
				throw std::string("Visitor step over all nodes\n");

			if (isVisit(current->getKey()))
				next();

			return current;
		}

		void reset() {
			current = root;
			visited.clear();
		}

		bool hasNext() { return visited.size() < this->count; }

		void next() {
			if (!hasNext())
				throw std::string("Do not find next element\n");

			if (!isVisit(current->getKey()))
				visited.push_back(current->getKey());

			if (current->getLeft() != nullptr) {
				if (!isVisit(current->getLeft()->getKey())) {
					current = current->getLeft();
					return;
				}
			}

			if (current->getRight() != nullptr) {
				if (!isVisit(current->getRight()->getKey())) {
					current = current->getRight();
					return;
				}
			}

			while (current != nullptr) {
				if (current->getLeft() != nullptr)
					if (!isVisit(current->getLeft()->getKey()))
						break;

				if (current->getRight() != nullptr)
					if (!isVisit(current->getRight()->getKey()))
						break;

				current = current->getParent();
			}
		}
	};

	BST::BST_Visitor visitor;

	// Получить предыдущую ноду дерева по ключу.
	BST::BST_Node* getPreviousNodeByKey(K key) {
		BST::BST_Node* temp = this->root;

		while (true) {
			if (key > temp->getKey()) {
				if (temp->getRight() != nullptr)
					temp = temp->getRight();
				else
					return temp;
			}
			else {
				if (temp->getLeft() != nullptr)
					temp = temp->getLeft();
				else
					return temp;
			}
		}
	}

	// Получить ноду дерева по ключу или нулевой указатель.
	BST::BST_Node* getNodeOrNull(K key) {
		BST::BST_Node* temp = this->root;

		while (temp != nullptr) {
			if (temp->getKey() == key)
				return temp;

			if (key > temp->getKey())
				temp = temp->getRight();
			else
				temp = temp->getLeft();
		}

		return temp;
	}

	// Утилита рекурсивно записывает в вектор все вхождения в дереве.
	void getAllEntrysUtil(BST::BST_Node* temp, std::vector<BST::BST_Entry>* result) {
		if (temp == nullptr)
			return;

		result->push_back(temp->getEntry());

		if (temp->getLeft() != nullptr)
			getAllEntrysUtil(temp->getLeft(), result);

		if (temp->getRight() != nullptr)
			getAllEntrysUtil(temp->getRight(), result);
	}

	// Утилита рекурсивно сравнивает вхождения по нодам двух деревьев и возвращает результат является ли дерево под-деревом.
	bool isSubtreeRecursionUtil(BST::BST_Node* left, BST::BST_Node* right) {
		if (left == nullptr && right == nullptr)
			return true;

		if (left == nullptr || right == nullptr)
			return false;

		return left->equals(right) && isSubtreeRecursionUtil(left->getLeft(), right->getLeft()) && isSubtreeRecursionUtil(left->getRight(), right->getRight());
	}

public:
	BST() {
		this->root = nullptr;
		this->size = 0;
	}

	BST(K key, V value) : BST(BST::BST_Entry(key, value)) { }

	BST(BST::BST_Entry entry) {
		this->root = new BST_Node(entry);
		this->size++;

		this->visitor = BST::BST_Visitor(this->root, this->size);
	}

	BST(std::vector<BST::BST_Entry> entrys) : BST() {
		for (int i = 0; i < entrys.size(); i++)
			this->add(entrys[i]);
	}

	bool isEmpty() { return this->root == nullptr; }

	unsigned long long int count() { return this->size; }

	bool containsKey(K key) {
		return this->getNodeOrNull(key) != nullptr;
	}

	bool add(K key, V value) {
		return this->add(BST::BST_Entry(key, value));
	}

	bool add(BST::BST_Entry entry) {
		if (this->isEmpty()) {
			this->root = new BST::BST_Node(entry);
			this->size++;
			this->visitor = BST::BST_Visitor(this->root, this->size);
			return true;
		}

		if (this->containsKey(entry.getKey()))
			return false;

		BST::BST_Node* temp = this->getPreviousNodeByKey(entry.getKey());
		this->visitor.addCount();
		this->size++;

		if (entry.getKey() > temp->getKey()) {
			temp->setRight(new BST::BST_Node(entry, temp));
			temp->getRight()->setParent(temp);
		}
		else {
			temp->setLeft(new BST::BST_Node(entry, temp));
			temp->getLeft()->setParent(temp);
		}

		return true;
	}

	BST::BST_Entry getRootEntry() {
		return this->isEmpty() ? throw string("Tree is empty\n") : this->root->getEntry();
	}

	BST::BST_Entry getEntry(K key) {
		BST::BST_Node* temp = this->getNodeOrNull(key);

		if (temp == nullptr)
			throw std::string("Key does not exist in the tree\n");

		return temp->getEntry();
	}

	V getValue(K key) {
		return this->getEntry(key).getValue();
	}

	std::vector<BST::BST_Entry> getAllEntrys(K fromKey = NULL) {
		std::vector<BST::BST_Entry> result = std::vector<BST::BST_Entry>();
		BST::BST_Node* temp = fromKey == NULL ? this->root : this->containsKey(fromKey) ? getNodeOrNull(fromKey) : throw string("Key does not exist in the tree\n");
		getAllEntrysUtil(temp, &result);
		return result;
	}

	bool erase(K key) {
		BST::BST_Node* temp = this->getNodeOrNull(key);

		if (temp == nullptr)
			return false;

		BST::BST_Node* parent = temp->getParent();
		BST::BST_Node* left = temp->getLeft();
		BST::BST_Node* right = temp->getRight();
		bool isLeft = parent->getLeft()->equals(temp) ? true : false;
		this->visitor.removeCount();
		this->size--;

		BST::BST_Node* toSet = left != nullptr ? left : right;
		if (left != nullptr && right != nullptr) {
			while (left->getRight() != nullptr)
				left = left->getRight();

			left->setRight(right);
		}

		if (parent == nullptr) {
			this->root = toSet;
		}
		else {
			if (isLeft)
				parent->setLeft(toSet);
			else
				parent->setRight(toSet);
		}

		return true;
	}

	bool visitorHasNext() { return this->visitor.hasNext(); }

	void visitorNext() { this->visitor.next(); }

	void visitorReset() { this->visitor.reset(); }

	BST::BST_Entry visitorGetEntry() {
		return this->visitor.get()->getEntry();
	}

	BST::BST_Node* visitorGet() {
		return this->visitor.get();
	}

	BST::BST_Node* getNodeByKey(K key) {
		return this->getNodeOrNull(key);
	}

	BST::BST_Node* getRootNode() {
		return this->getNodeByKey(this->root->getKey());
	}

	// Получить под-дерево из текущего, корень которого находится по ключу.
	BST getSubTree(K key) {
		if (!this->containsKey(key))
			throw std::string("Key does not exist in the tree\n");

		std::vector<BST::BST_Entry> subtree = this->getAllEntrys(key);
		return BST(subtree);
	}

	// Проверить является ли дерево под-деревом с помощью визитера дерева
	bool isSubtree(BST subtree) {
		if (this->root == nullptr || subtree.getRootNode() == nullptr)
			return false;

		BST currentSubtree = this->getSubTree(subtree.getRootEntry().getKey());

		while (subtree.visitorHasNext() && currentSubtree.visitorHasNext()) {
			if (subtree.visitorGetEntry().getKey() != currentSubtree.visitorGetEntry().getKey())
				return false;

			subtree.visitorNext(); currentSubtree.visitorNext();
		}

		return true;
	}

	// Проверить является ли дерево под-деревом с помощью рекурсивной утилиты
	bool isSubtreeRecursion(BST subtree) {
		if (this->root == nullptr || subtree.getRootNode() == nullptr)
			return false;

		BST::BST_Node* subtreeNode = subtree.getRootNode();
		BST::BST_Node* currentNode = this->getNodeByKey(subtreeNode->getKey());

		return currentNode == nullptr || subtreeNode == nullptr ? false : this->isSubtreeRecursionUtil(subtreeNode, currentNode);
	}
};

// Выполнить тест и записать входные данные и результат.
void test_unit(BST<int, string> source, BST<int, string> sample, string sourceData, string sampleData) {
	std::cout << std::endl << "Source BST: " << sourceData << std::endl << "Sample BST: " << sampleData;
	std::cout << std::endl << std::boolalpha << "Is subtree: " << source.isSubtree(sample);
	std::cout << std::endl << std::boolalpha << "Is subtree recursion: " << source.isSubtreeRecursion(sample) << std::endl;
}

// Получить дерево из строки.
BST<int, string> bstFromString(string data) {
	BST<int, string> result = BST<int, string>();
	std::stringstream ss(data);
	std::string item;

	while (std::getline(ss, item, ' '))
		result.add(stoi(item), item);

	return result;
}

// Взять данные из файла и запустить тест.
void test_from_file(filesystem::directory_entry file) {
	std::ifstream fileStream = std::ifstream(file.path(), std::ios_base::in);
	std::string sourceData, sampleData;
	std::getline(fileStream, sourceData);
	std::getline(fileStream, sampleData);

	fileStream.close();
	test_unit(bstFromString(sourceData), bstFromString(sampleData), sourceData, sampleData);
}

// Запустить тест для каждого тестового набора из файла в директории.
void run_tests(string tests_directory) {
	try {
		for (const filesystem::directory_entry file : filesystem::directory_iterator(tests_directory))
			test_from_file(file);
	}
	catch (filesystem::filesystem_error exp) {
		std::cout << exp.what() << std::endl;
	}
}

int main() {
	run_tests("tests");

	return 0;
}