#pragma once
#include<iostream>
#include<math.h>
#include<queue>

enum color { black, red, nil };
struct node {
	int key = 0;
	node* parent = nullptr;
	node* left = nullptr;
	node* right = nullptr;
	color color = nil;
};

class Tree {
protected:
	node* root = nullptr;

private:
	void DLR(node* current);
	void LDR(node* current);
	void LRD(node* current);
	void clear(node* current);

	int getHeight(node * current);
	void printBlank(int num);
public:
	Tree() {}
	~Tree() { clear(root);}

	virtual node* insert(const int &key) = 0;
	virtual int remove(const int &key) = 0;
	virtual node* search(const int &key) = 0;

	void printDLR() { DLR(root); std::cout << std::endl; }
	void printLDR() { LDR(root); std::cout << std::endl; }
	void printLRD() { LRD(root); std::cout << std::endl; }

	void printTree();
};
void Tree::DLR(node* current) {
	if (root==nullptr || current == nullptr)
		return;

	std::cout << current->key <<current->color<< " ";
	DLR(current->left);
	DLR(current->right);

	return;
}
void Tree::LDR(node* current) {
	if (root==nullptr || current == nullptr)
		return;

	LDR(current->left);
	std::cout << current->key <<current->color<< " ";
	LDR(current->right);

	return;
}
void Tree::LRD(node* current) {
	if (root==nullptr || current == nullptr)
		return;

	LRD(current->left);
	LRD(current->right);
	std::cout << current->key <<current->color<< " ";

	return;
}
void Tree::clear(node* current) {
	if (current == nullptr)
		return;

	clear(current->left);
	clear(current->right);

	delete current;
	current = nullptr;

	return;
}

void Tree::printBlank(int num) {
	for (int i = 0; i < num; ++i)
		std::cout << " ";
	return;
}
int Tree::getHeight(node* current) {
	if (current == nullptr)
		return 0;

	int leftHeight = getHeight(current->left);
	int rightHeight = getHeight(current->right);

	if (leftHeight > rightHeight)
		return leftHeight + 1;
	else
		return rightHeight + 1;
}
void Tree::printTree() {
	int height = getHeight(root);
	int baseNum = pow(2, height);
	int step = baseNum - 1;

	std::deque<node*> nodeInFloor;
	for (int floor = 1; floor <= height; ++floor) {
		if (floor == 1) {
			nodeInFloor.push_back(root);
		}
		else {
			for (int i = 0; i < pow(2, floor - 2); ++i) {
				if (nodeInFloor.front() == nullptr) {
					nodeInFloor.push_back(nullptr);
					nodeInFloor.push_back(nullptr);
					nodeInFloor.pop_front();
				}
				else {
					nodeInFloor.push_back(nodeInFloor.front()->left);
					nodeInFloor.push_back(nodeInFloor.front()->right);
					nodeInFloor.pop_front();
				}
			}
		}

		printBlank((step + 1) / 2 - 1);

		for (int i = 0; i < pow(2, floor - 1); ++i) {
			if (nodeInFloor[i] == nullptr)
				std::cout << " ";
			else
				std::cout << nodeInFloor[i]->key;

			printBlank(step);
		}
		
		std::cout << std::endl;

		step = (step + 1)/2 - 1;
	}

	nodeInFloor.clear();

	return;
}


class BinarySearchTree :public Tree {
private:
	void insepctDLR(node* current);
public:
	BinarySearchTree() {}
	~BinarySearchTree() {}

	node* insert(const int &key)override;
	int remove(const int &key)override;
	node* search(const int &key)override;

	void selfInspection();
};
void BinarySearchTree::insepctDLR(node* current) {
	if (root == nullptr || current == nullptr)
		return;

	if (current->left != nullptr && current->left->key > current->key)
		std::cout << current->key << "'s left is larger than itself!" << std::endl;
	if (current->right != nullptr && current->right->key <= current->key)
		std::cout << current->key << "'s right is smaller than itself!" << std::endl;

	insepctDLR(current->left);
	insepctDLR(current->right);

	return;
}
node* BinarySearchTree::insert(const int &key) {


	if (root == nullptr) {
		root = new node;
		root->key = key;
		root->color = red;
		return root;
	}

	node* current = root;
	while (current != nullptr) {
		if (key <= current->key) {
			if (current->left == nullptr) {

				current->left = new node;
				current->left->key = key;
				current->left->parent = current;
				current->left->color = red;

				return current->left;
			}
			else
				current = current->left;
		}

		else {
			if (current->right == nullptr) {

				current->right = new node;
				current->right->key = key;
				current->right->parent = current;
				current->right->color = red;

				return current->right;
			}
			else
				current = current->right;
		}
	}

	return nullptr;
}
int BinarySearchTree::remove(const int &key) {
	if (root == nullptr)
		return 1;

	node* current = search(key);
	if (current == nullptr)
		return 1;

	if (current->left == nullptr&&current->right == nullptr) {
		if (current->parent != nullptr) {
			if (current->parent->right == current) {
				if (current->parent != nullptr)
					current->parent->right = nullptr;
			}

			else {
				current->parent->left = nullptr;
			}
		}
		else
			root = nullptr;
	}

	else if (current->left != nullptr&&current->right == nullptr) {
		current->left->parent = current->parent;

		if (current->parent != nullptr) {
			if (current->parent->right == current) 
				current->parent->right = current->left;
			else 
				current->parent->left = current->left;
		}
		else
			root = current->left;
	}

	else if (current->left == nullptr&&current->right != nullptr) {
		current->right->parent = current->parent;

		if (current->parent != nullptr) {
			if (current->parent->right == current) {
				current->parent->right = current->right;
			}

			else {
				current->parent->left = current->right;
			}
		}
		else
			root = current->right;
	}

	else if (current->left != nullptr&&current->right != nullptr) {
		node* successor = current->right;
		while (successor->left != nullptr)
			successor = successor->left;

		if (successor->right != nullptr)
			successor->right->parent = successor->parent;
		if (successor->parent->right == successor)
			successor->parent->right = successor->right;
		else
			successor->parent->left = successor->right;

		current->left->parent = successor;
		if (current->right != nullptr)
			current->right->parent = successor;
		if (current->parent != nullptr) {
			if (current->parent->right == current)
				current->parent->right = successor;
			else
				current->parent->left = successor;
		}
		else
			root = successor;
		
		successor->parent = current->parent;
		successor->left = current->left;
		successor->right = current->right;
	}

	delete current;
	current=nullptr;

	return 1;
}
node* BinarySearchTree::search(const int &key) {
	if (root == nullptr)
		return nullptr;

	node* current = root;

	while (current->key != key && current != nullptr) {
		if (key > current->key)
			current = current->right;
		else {
			current = current->left;
		}
	}

	return current;
}
void BinarySearchTree::selfInspection() {
	if (root == nullptr) {
		std::cout << "Tree is empty!" << std::endl;
		return;
	}

	insepctDLR(root);
	return;
}


class RedBlackTree :public BinarySearchTree {
private:
	int leftRatate(node* current);
	int rightRatate(node* current);

	int transplant(node* origin, node* fresher);
	int gekokujo(node* origin, node* fresher);
	int removeFixup(node* current);

	int BHDLR(node* current);
public:
	RedBlackTree(){}
	~RedBlackTree(){}

	node* insert(const int&key);
	int remove(const int&key);

	void selfInseption();
};
int RedBlackTree::leftRatate(node* current) {
	if (current->right == nullptr || current == nullptr)
		return 1;

	node* right = current->right;

	if (current->parent != nullptr) {
		if (current->parent->right == current)
			current->parent->right = right;
		else
			current->parent->left = right;
	}
	else
		root = right;

	if (right->left != nullptr)
		right->left->parent = current;

	right->parent = current->parent;
	current->right = right->left;
	current->parent = right;
	right->left = current;

	return 0;
}
int RedBlackTree::rightRatate(node* current) {
	if (current->left == nullptr || current == nullptr)
		return 1;
	node* left = current->left;

	if (current->parent != nullptr) {
		if (current->parent->right == current)
			current->parent->right = left;
		else
			current->parent->left = left;
	}
	else
		root = left;

	if (left->right != nullptr)
		left->right->parent = current;

	left->parent = current->parent;
	current->parent = left;
	current->left = left->right;
	left->right = current;

	return 0;
}

int RedBlackTree::gekokujo(node* origin, node* fresher) {
	if (origin == nullptr)
		return 1;

	if (fresher == nullptr || fresher->parent == origin) {

		if (origin->parent == nullptr)
			root = fresher;
		else {
			if (origin->parent->right == origin)
				origin->parent->right = fresher;
			else
				origin->parent->left = fresher;
		}
		if (fresher != nullptr)
			fresher->parent = origin->parent;

		if (origin->right == fresher) {
			if (origin->left != nullptr)
				origin->left->parent = fresher;
			if (fresher != nullptr)
				fresher->left = origin->left;
		}
		else {
			if (origin->right != nullptr)
				origin->right->parent = fresher;
			if (fresher != nullptr)
				fresher->right = origin->right;
		}

		return 0;
	}
	else
		return 1;
}
int RedBlackTree::removeFixup(node* current) {
	if (current == nullptr)
		return 1;

	while (current != root && current->color == black) {
		if (current->parent->right == current) {
			node*brother = current->parent->left;

			if (brother->color == red) {
				brother->color = black;
				current->parent->color = red;
				rightRatate(current->parent);
				brother = current->parent->left;
			}

			if ((brother->left == nullptr || brother->left->color == black) && (brother->right == nullptr || brother->right->color == black)) {
				brother->color = red;
				current = current->parent;
			}
			
			else {
				if (brother->left == nullptr || brother->left->color == black) {
					brother->color = red;
					brother->right->color = black;
					leftRatate(brother);
					brother = current->parent->left;
				}

				brother->left->color = black;
				brother->color = current->parent->color;
				current->parent->color = black;
				rightRatate(current->parent);

				current = root;
			}
		}

		else {
			node* brother = current->parent->right;

			if (brother->color == red) {
				brother->color = black;
				current->parent->color = red;
				leftRatate(current->parent);
				brother = current->parent->right;
			}
			
			if ((brother->left == nullptr || brother->left->color == black) && (brother->right == nullptr || brother->right->color == black)) {
				brother->color = red;
				current = current->parent;
			}

			else {
				if (brother->right == nullptr || brother->right->color == black) {
					brother->color = red;
					brother->left->color = black;
					rightRatate(brother);
					brother = current->parent->right;
				}

				brother->color = current->parent->color;
				current->parent->color = black;
				brother->right->color = black;
				leftRatate(current->parent);
				current = root;
			}
		}
	}

	if(current!=nullptr)
		current->color = black;

	return 0;
}
int RedBlackTree::BHDLR(node*current) {
	if (current == nullptr)
		return 1;

	if (current->color == red && current->left != nullptr && current->left->color == red)
		std::cout << current->key << "and its left are borh red!" << std::endl;
	if (current->color == red && current->right != nullptr && current->right->color == red)
		std::cout << current->key << "and its right are both red!" << std::endl;

	int leftBH = BHDLR(current->left);
	int rightBH = BHDLR(current->right);

	if (leftBH != rightBH)
		std::cout << "Black high error at" <<current->key<<"!"<< std::endl;

	if (current->color == red)
		return leftBH;
	else
		return leftBH + 1;
}
void RedBlackTree::selfInseption() {
	if (root == nullptr) {
		std::cout << "Tree is empty!" << std::endl;
		return;
	}
	
	BinarySearchTree::selfInspection();

	if (root->color != black) {
		std::cout << "root isn't black!" << std::endl;
	}
	
	BHDLR(root);

	return;
}

node* RedBlackTree::insert(const int&key) {
	node* current=BinarySearchTree::insert(key);
	if (current == nullptr)
		return nullptr;

	if (current->parent == nullptr) {
		current->color = black;
		return current;
	}

	while (current->parent!=nullptr && current->parent->color == red) {
		if (current->parent->parent->right == current->parent) {
			node* uncle = current->parent->parent->left;

			if (uncle != nullptr && uncle->color == red) {
				current->parent->color = black;
				uncle->color = black;
				current->parent->parent->color = red;
				current = current->parent->parent;
			}

			else {
				if (current->parent->left == current) {
					current = current->parent;
					rightRatate(current);
				}

				current->parent->color = black;
				current->parent->parent->color = red;
				leftRatate(current->parent->parent);
			}
		}

		else {
			node* uncle = current->parent->parent->right;

			if (uncle!=nullptr && uncle->color == red) {
				uncle->color = black;
				current->parent->color = black;
				current->parent->parent->color = red;
				current = current->parent->parent;
			}

			else {
				if (current->parent->right == current) {
					current = current->parent;
					leftRatate(current);
				}

				current->parent->color = black;
				current->parent->parent->color = red;
				rightRatate(current->parent->parent);
			}
		}

		root->color = black;
	}
	

	return current;
}
int RedBlackTree::remove(const int& key) {
	if (root == nullptr)
		return 1;

	node* goal = search(key);
	if (goal == nullptr)
		return 1;

	color originColor = goal->color;

	if (goal->left != nullptr && goal->right != nullptr) {
		node* successor = goal->right;
		while (successor->left != nullptr)
			successor = successor->left;

		goal->key = successor->key;

		goal = successor;
		originColor = goal->color;
	}
	
	if (goal->left == nullptr && goal->right == nullptr) {

		if (goal->parent == nullptr) {
			delete goal;
			goal = nullptr;
			root = nullptr;

			return 0;
		}

		node * replacer = goal;
		if (originColor == black)
			removeFixup(goal);

		if (goal->parent->right==goal)
			goal->parent->right = nullptr;
		else
			goal->parent->left = nullptr;
	}

	else {
		node* replacer = nullptr;
		color originColor = goal->color;

		if (goal->right == nullptr) {
			replacer = goal->left;
			gekokujo(goal, replacer);
		}
		else {
			replacer = goal->right;
			gekokujo(goal, replacer);
		}

		if (originColor == black)
			removeFixup(replacer);
	}

	delete goal;
	goal = nullptr;

	return 0;
}


