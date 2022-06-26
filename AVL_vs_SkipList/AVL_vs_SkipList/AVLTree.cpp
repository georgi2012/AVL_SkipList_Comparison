#include "AVLTree.h"

bool AVLTree::exists(int key) const noexcept
{
	return findNode(key, root) != nullptr;
}

void AVLTree::clearData() noexcept
{
	deleteAll(root);
	root = nullptr;
	size = 0;
}

AVLIterator AVLTree::begin() const noexcept
{
	return AVLIterator(root);
}

AVLIterator AVLTree::end() const noexcept
{
	return AVLIterator(nullptr);
}

Node* AVLTree::leftRotate(Node* node) noexcept
{
	if (!node->right) return node;
	Node* rightNode = node->right;
	Node* farLeft = rightNode->left;
	rightNode->left = node;
	node->right = farLeft;
	return rightNode;
}

Node* AVLTree::balanceTreeInsertion(const int val, Node* node) noexcept
{
	int balance = height(node->left) - height(node->right);
	if (balance > 1 && val < node->left->value) {
		return rightRotate(node);// Left Left Case
	}
	if (balance < -1 && val > node->right->value) {
		return leftRotate(node);// Right Right Case
	}
	if (balance > 1 && val > node->left->value) {
		node->left = leftRotate(node->left);// Left Right Case
		return rightRotate(node);
	}
	if (balance < -1 && val < node->right->value) {
		node->right = rightRotate(node->right);// Right Left Case
		return leftRotate(node);
	}
	return node;
}


Node* AVLTree::balanceTreeDeletion(const int val, Node* node) noexcept
{
	int bLeft = height(node->left);
	int bRight = height(node->right);
	int balance = bLeft - bRight;
	if (balance > 1 && val && bLeft >= 0) {
		return rightRotate(node);// Left Left Case
	}
	if (balance < -1 && val && bRight <= 0) {
		return leftRotate(node);// Right Right Case
	}
	if (balance > 1 && val && bLeft < 0) {
		node->left = leftRotate(node->left);// Left Right Case
		return rightRotate(node);
	}
	if (balance < -1 && val && bLeft > 0) {
		node->right = rightRotate(node->right);// Right Left Case
		return leftRotate(node);
	}
	return node;
}


Node* AVLTree::rightRotate(Node* node) noexcept
{
	if (!node->left) return node;
	Node* leftNode = node->left;
	Node* farRight = leftNode->right;
	leftNode->right = node;
	node->left = farRight;
	return leftNode;
}

Node* AVLTree::insertNode(const int val, Node* node)
{
	if (node == nullptr) {
		return new Node(val);
	}
	if (val < node->value) {
		node->left = insertNode(val, node->left);
	}
	else if (val > node->value) {
		node->right = insertNode(val, node->right);
	}
	else {//equal not permitted
		throw std::logic_error("Node already exists");
	}
	return balanceTreeInsertion(val, node); //!balancing part!
}

Node* AVLTree::deleteNode(const int val, Node* node) noexcept {
	if (!node)
		return node;
	if (val < node->value) {
		node->left = deleteNode(val, node->left);
	}
	else if (val > node->value) {
		node->right = deleteNode(val, node->right);
	}
	else
	{
		//one child cases
		if (!node->left || !node->right)
		{
			Node* temp = node->left ? node->left : node->right;
			if (!temp)
			{
				temp = node;
				node = nullptr;
			}
			else
			{
				*node = *temp;
			}
			if (temp == root) {
				delete root;
				root = nullptr;
			}
			else {
				delete temp;
			}
			--size;
		}
		else
		{
			Node* temp = getMin(node->right);
			node->value = temp->value;
			node->right = deleteNode(temp->value, node->right);
		}
	}
	if (!node)
		return node;
	return balanceTreeDeletion(val, node);

}

Node* AVLTree::getMin(Node* node) const noexcept {
	if (node->left) {
		return getMin(node->left);
	}
	Node* lastRoot = node;
	node = node->right;
	return lastRoot;

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

int AVLTree::height(const Node* node) const noexcept
{
	if (!node) return 0;
	int leftH = height(node->left);
	int rightH = height(node->right);
	return 1 + (leftH > rightH ? leftH : rightH);
}

Node* AVLTree::findNode(const int val, Node* node) const noexcept {
	if (!node) return nullptr;
	if (node->value == val) return node;
	return val < node->value ? findNode(val, node->left) : findNode(val, node->right);
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
		size = 0;
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
		size = other.getSize();
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

bool AVLTree::remove(int key) noexcept
{
	auto oldSize = size;
	deleteNode(key, root);
	return oldSize > size;
}

bool AVLTree::insert(int key) noexcept
{
	try {
		root = insertNode(key, root);
		++size;
	}
	catch (...) {
		return false;
	}
	return true;
}

size_t AVLTree::getHeight() const noexcept {
	return height(root);
}

AVLIterator AVLIterator::operator++()
{
	if (nodes.empty()) {
		return AVLIterator(nullptr);
	}
	Node* node = nodes.top();
	nodes.pop();
	if (node->right) nodes.push(node->right);
	if (node->left) nodes.push(node->left);
	return *this;
}

AVLIterator::AVLIterator(Node* firstNode) noexcept
{
	if (!firstNode) return;
	nodes.push(firstNode);

}

const Node* AVLIterator::operator*() const
{
	if (nodes.empty()) return nullptr;
	return nodes.top();
}


bool AVLIterator::operator==(const AVLIterator& other) const noexcept {
	return operator*() == *other;
}

bool AVLIterator::operator!=(const AVLIterator& other) const noexcept {
	return operator*() != *other;
}