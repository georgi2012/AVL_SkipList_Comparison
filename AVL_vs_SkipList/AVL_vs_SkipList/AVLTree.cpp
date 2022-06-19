#include "AVLTree.h"

void AVLTree::leftRotate(Node*& node) noexcept
{
	

}

void AVLTree::rightRotate(Node*& node) noexcept
{
	
}

void AVLTree::correctInsertion(Node*& node) noexcept
{
	
}

void AVLTree::deleteAll(Node* node) noexcept
{
	if (!node) return; //for when root is nullptr
	if (node->left) {
		deleteAll(node->left);
	}
	if (node->right) {
		deleteAll(node->right);
	}
	delete node;
}

bool AVLTree::removeNode(Node* node) noexcept
{
	
}


Node* AVLTree::makeCopy(const Node* current)
{
	if (!current) return nullptr;
	Node* node = nullptr;
	node = new Node(current->value);//may throw
	try {
		node->left = makeCopy(current->left);
	}
	catch (...) {
		delete node;
		throw;
	}
	try {
		node->right = makeCopy(current->right);
	}
	catch (...) {
		delete node;
		deleteAll(node->left);
		throw;
	}
	return node;
}


AVLTree::AVLTree(const AVLTree& other)
{
	root = makeCopy(other.root);
	//if root=nullptr -> problem with allocation
}

AVLTree::AVLTree(AVLTree&& other) noexcept
	:AVLTree()
{
	std::swap(other.size, this->size);
	std::swap(other.root, this->root);

}

AVLTree& AVLTree::operator=(const AVLTree& other)
{
	if (&other != this) {
		Node* newRoot = makeCopy(other.root);
		deleteAll(root);
		root = newRoot;
	}
	return *this;
}

AVLTree& AVLTree::operator=(AVLTree&& other) noexcept
{

	if (&other != this) {
		deleteAll(root);
		size = 0;
		std::swap(other.size, this->size);
		std::swap(other.root, this->root);
	}
	return *this;
}

AVLTree::~AVLTree() noexcept
{
	if (root) {
		deleteAll(root);
		size = 0;
		root = nullptr;
	}
}

size_t AVLTree::getSize() const noexcept
{
	return size;
}

bool AVLTree::insert(int key) noexcept
{
	Node* newNode = nullptr;
	try {
		newNode = new Node(key);
	}
	catch (...) {
		return false;//couldnt insert 
	}
	if (!root) {
		root = newNode;
	}
	else {
		//find the right parent
		findPlace(newNode);
		// fix RB rules
		correctInsertion(newNode);
	}
	++size;
	return true;
}

inline void AVLTree::findPlace(Node* newNode) noexcept
{
	Node* node = root;
	Node* parent = nullptr;
	while (node)
	{
		parent = node;
		if (newNode->value < node->value) {
			node = node->left;
		}
		else {
			node = node->right;
		}
	}
	//newNode->parent = parent;
	if (newNode->value < parent->value) { //place in the correct side
		parent->left = newNode;
	}
	else {
		parent->right = newNode;
	}
}

bool AVLTree::remove(int key) noexcept
{
	//we should find it
	Node* helper = root;
	//
	while (helper) {
		if (helper->value == key) {
			break;
		}
		if (key < helper->value) {
			if (!helper->left) break;
			helper = helper->left;
		}
		else {
			if (!helper->right) break;
			helper = helper->right;
		}
	}
	if (helper && helper->value == key && removeNode(helper)) {
		--size;
		return true;
	}
	return false;//couldnt find

}

bool AVLTree::exists(int key) const noexcept
{
	Node* node = root;
	while (node) {
		if (node->value == key) {
			return true;
		}
		if (node->value > key) {
			node = node->left;
		}
		else {
			node = node->right;
		}
	}
	return false;
}
