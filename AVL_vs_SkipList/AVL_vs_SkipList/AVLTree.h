#pragma once
#include <iostream>
#include <cstdint>
/*
RB rules:
1.Every node is either red or black.
2.All nullptr (leaves) are considered black.
3.A red node does not have a red child.
4.Every path from a given node to any of its descendant leaves goes through the same number of black nodes.
(5.The root is black.)
*/
//make template?

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
	void leftRotate(Node*& node) noexcept;
	void rightRotate(Node*& node) noexcept;
	void correctInsertion(Node*& node) noexcept;
	void deleteAll(Node* node) noexcept;
	bool removeNode(Node* node) noexcept;
	//void correctRemoving(Node* node) noexcept;
	inline Node* getSibling(Node* node) const noexcept;
	Node* makeCopy(const Node* current);
	inline void findPlace(Node* newNode) noexcept;
public:
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
};

class RBIterator {
private:


public:
	friend class AVLTree;


};