#pragma once
#include <iostream>
#include <stack>

//template?
class AVLIterator;

struct Node {
	Node* left, * right;
	int value;

	Node(const int value, Node* left = nullptr, Node* right = nullptr)
		: value(value), left(left), right(right) {}
};

class AVLTree {
private:
	//data
	Node* root = nullptr;
	size_t size = 0;

	//private methods
	Node* makeCopy(const Node* current);
	void deleteAll(Node* node) noexcept;
	Node* getMin(Node* node) const noexcept;
	int height(const Node* node) const noexcept;
	Node* findNode(const int val, Node* node) const noexcept;
	Node* insertNode(const int val, Node* node);
	Node* deleteNode(const int val, Node* node) noexcept;
	Node* balanceTree(const int val, Node* node) noexcept;
	Node* rightRotate(Node* node) noexcept;
	Node* leftRotate(Node* node) noexcept;
public:
	friend class AVLIterator;
	//constructors and operators
	AVLTree() = default;
	AVLTree(const AVLTree& other);
	AVLTree(AVLTree&& other) noexcept;
	AVLTree& operator=(const AVLTree& other);
	AVLTree& operator=(AVLTree&& other) noexcept;
	~AVLTree() noexcept;
	//public methods
	size_t getSize() const noexcept;
	bool insert(int key) noexcept;
	bool remove(int key) noexcept;
	bool exists(int key) const noexcept;
	size_t getHeight() const noexcept;
	void clearData() noexcept;
	//iteration
	AVLIterator begin() const noexcept;
	AVLIterator end() const noexcept;
};

//left-parent-right
class AVLIterator {
private:
	//data
	std::stack<Node*> nodes;
	//methods
	AVLIterator(Node* firstNode) noexcept;
public:
	friend class AVLTree;
	//methods
	AVLIterator  operator++();
	const Node* operator*() const;
	bool operator==(const AVLIterator& other) const noexcept;
	bool operator!=(const AVLIterator& other) const noexcept;

};