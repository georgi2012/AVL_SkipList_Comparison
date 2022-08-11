#pragma once
#include <iostream>
#include <stack>

template <class>
class AVLIterator;

/// @brief Self-balancing AVL tree with no repetitions rule
template <class T>
class AVLTree {
private:
	/// @brief Tree node structure that keeps value and children pointers
	struct Node {
		/// @brief Value of the node 
		T value;
		/// @brief Node pointers for the children
		Node* left, * right;
		/// @brief Height of the subtree of that node 
		size_t height = 0;
		/// @brief Default constructor that sets value
		Node(const T& value, Node* left = nullptr, Node* right = nullptr)
			: value(value), left(left), right(right) {}
	};
	//data
	/// @brief Node pointer to the root
	Node* root = nullptr;
	/// @brief Number of nodes in the tree
	size_t size = 0;
	//private methods

	/// @brief Method to copy a tree
	/// @param current Start node for the copying
	/// @return Root of the new tree
	Node* makeCopy(const Node* current);
	/// @brief Method to delete all nodes in a tree. Does not set size
	/// @param node Starting node for the deletion
	void deleteAll(Node* node) noexcept;
	/// @brief Method to return the smallest element and change it's place with his right child
	/// @param node Starting node for the search
	/// @return Smallest node
	Node* getMin(Node* node) const noexcept;
	/// @brief Method to return the height of the tree
	/// @param node Starting node for the counting
	/// @return Height
	int height(const Node* node) const noexcept;
	/// @brief Method to return the node with specific value
	/// @param val Value to be searched
	/// @param node Starting node for the counting
	/// @return Pointer to the node with such value or nullptr if there isn't such
	Node* findNode(const T& val, Node* node) const noexcept;
	/// @brief Method to insert a node with specific value
	/// @param val Value to be added
	/// @param node Starting node for the adding
	/// @return Pointer to the last used node
	Node* insertNode(const T& val, Node* node);
	/// @brief Method to remove a node with specific value
	/// @param val Value to be removed
	/// @param node Starting node for the adding
	/// @return Pointer to the last used node
	Node* deleteNode(const T& val, Node* node) noexcept;
	/// @brief Method to balance the tree when inserting and deleting nodes
	/// @param val Value that has been changed lastly
	/// @param node Starting node for the balance
	/// @return Pointer to the last rotated node
	Node* balanceTree(const T& val, Node* node) noexcept;
	/// @brief Standart Right rotation for the specific node
	Node* rightRotate(Node* node) noexcept;
	/// @brief Standart Left rotation for the specific node
	Node* leftRotate(Node* node) noexcept;
public:
	friend class AVLIterator<T>;
	//constructors and operators
	/// @brief Standart constructor creating empty tree
	AVLTree() = default;
	/// @brief Copy constructor. Uses MakeCopy()
	/// @param other Tree to be copied
	AVLTree(const AVLTree& other);
	/// @brief Move constructor.
	/// @param other Tree to be moved. Moved object will be deleted
	AVLTree(AVLTree&& other) noexcept;
	/// @brief Copy operator. Uses MakeCopy()
	/// @param other Tree to be copied
	AVLTree& operator=(const AVLTree& other);
	/// @brief Move operator.
	/// @param other Tree to be moved. Moved object will be deleted
	AVLTree& operator=(AVLTree&& other) noexcept;
	/// @brief Destructor . Uses DeleteAll() and sets size to 0.
	~AVLTree() noexcept;
	//public methods
	/// @brief Returns the number of nodes in the tree
	size_t getSize() const noexcept;
	/// @brief Inserts element through insertNode() method with specific key. Returns if operation was successful
	bool insert(const T& key) noexcept;
	/// @brief Removes element through deleteNode() method with specific key. Returns if operation was successful
	bool remove(const T& key) noexcept;
	/// @brief Returns if a node with such key exists. Uses findNode()
	bool exists(const T& key) const noexcept;
	/// @brief Returns tree height
	size_t getHeight() const noexcept;
	/// @brief Deleted all nodes and sets size to 0
	void clearData() noexcept;
	//iteration
	/// @brief Returns iterator to the start (root) of the tree
	AVLIterator<T> begin() const noexcept;
	/// @brief Returns iterator to the end (nullptr) of the tree
	AVLIterator<T> end() const noexcept;
	/// @brief Returns the memory used by the structure in bytes
	size_t getBytesUsed() const noexcept;
	/// @brief Returns height of left side minus height of right
	inline short getBalance(Node* node) const noexcept;
};

/// @brief AVL Tree iterator using left-parent-right traversal
template <class T>
class AVLIterator {
private:
	//data
	/// @brief Stack of nodes to keep the next node in the order
	std::stack<typename AVLTree<T>::Node*> nodes;
	//methods
	/// @brief Constructor that pushes the given node as root of the traversal
	AVLIterator(typename AVLTree<T>::Node* firstNode) noexcept;
public:
	friend class AVLTree<T>;
	//methods
	/// @brief Operator to move the stack to the next node in the order.
	AVLIterator  operator++();
	/// @brief Operator to get the next Node in the order
	const typename AVLTree<T>::Node* operator*() const;
	/// @brief Operator to check if two Iterators are the same
	bool operator==(const AVLIterator<T>& other) const noexcept;
	/// @brief Operator to check if two Iterators are not the same
	bool operator!=(const AVLIterator<T>& other) const noexcept;

};

//impl

template<class T>
inline short AVLTree<T>::getBalance(Node* node) const noexcept
{
	return node ? height(node->left) - height(node->right) : 0;
}

template<class T>
bool AVLTree<T>::exists(const T& key) const noexcept
{
	return findNode(key, root) != nullptr;
}

template<class T>
void AVLTree<T>::clearData() noexcept
{
	deleteAll(root);
	root = nullptr;
	size = 0;
}

template<class T>
AVLIterator<T> AVLTree<T>::begin() const noexcept
{
	return AVLIterator<T>(root);
}

template<class T>
AVLIterator<T> AVLTree<T>::end() const noexcept
{
	return AVLIterator<T>(nullptr);
}

template<class T>
typename AVLTree<T>::Node* AVLTree<T>::leftRotate(AVLTree<T>::Node* node) noexcept
{
	if (!node || !node->right) return node;
	AVLTree<T>::Node* rightNode = node->right;
	AVLTree<T>::Node* farLeft = rightNode->left;
	rightNode->left = node;
	node->right = farLeft;
	node->height = 1 + std::max(height(node->left), height(node->right));
	rightNode->height = 1 + std::max(height(rightNode->left), height(rightNode->right));
	return rightNode;
}


template<class T>
size_t AVLTree<T>::getBytesUsed() const noexcept
{
	return size * sizeof(Node) + sizeof(AVLTree<T>);
}

template<class T>
typename AVLTree<T>::Node* AVLTree<T>::balanceTree(const T& val, AVLTree<T>::Node* node) noexcept
{
	if (!node) return nullptr;
	int bLeft = getBalance(node->left);
	int bRight = getBalance(node->right);
	int balance = getBalance(node);
	if (balance > 1 && bLeft >= 0) {
		return rightRotate(node);// Left Left Case
	}
	if (balance < -1 && bRight <= 0) {
		return leftRotate(node);// Right Right Case
	}
	if (balance > 1 && bLeft < 0) {
		node->left = leftRotate(node->left);// Left Right Case
		return rightRotate(node);
	}
	if (balance < -1 && bLeft > 0) {
		node->right = rightRotate(node->right);// Right Left Case
		return leftRotate(node);
	}
	return node;
}

template<class T>
typename AVLTree<T>::Node* AVLTree<T>::rightRotate(AVLTree<T>::Node* node) noexcept
{
	if (!node || !node->left) return node;
	AVLTree<T>::Node* leftNode = node->left;
	AVLTree<T>::Node* farRight = leftNode->right;
	leftNode->right = node;
	node->left = farRight;
	
	node->height = std::max(height(node->left), height(node->right));
	leftNode->height = std::max(height(leftNode->left), height(leftNode->right));
	return leftNode;
}

template<class T>
typename AVLTree<T>::Node* AVLTree<T>::insertNode(const T& val, AVLTree<T>::Node* node)
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
	node->height = 1 + std::max(height(node->left), height(node->right));

	if (std::abs(height(node->left) - height(node->right)) > 1)
		return balanceTree(val, node); //!balancing part!
	else return node;
}

template<class T>
typename AVLTree<T>::Node* AVLTree<T>::deleteNode(const T& val, AVLTree<T>::Node* node) noexcept {
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
			AVLTree<T>::Node* temp = node->left ? node->left : node->right;
			if (!temp)
			{
				temp = node;
				node = nullptr;
			}
			else
			{
				*node = *temp; //copy values
			}
			if (temp == root) {
				delete root;
				root = nullptr;
			}
			else {
				delete temp;
			}
			temp = nullptr;
			--size;
		}
		else
		{
			AVLTree<T>::Node* temp = getMin(node->right);
			node->value = temp->value;
			node->right = deleteNode(temp->value, node->right);
		}
	}
	if (!node)
		return node;
	//height update
	node->height = 1 + std::max(height(node->left), height(node->right));

	//return 
	if (std::abs(height(node->left) - height(node->right)) > 1)
		return balanceTree(val, node); //!balancing part!
	else return node;

}

template<class T>
typename AVLTree<T>::Node* AVLTree<T>::getMin(typename AVLTree<T>::Node* node) const noexcept {
	if (node->left) {
		return getMin(node->left);
	}
	AVLTree<T>::Node* lastRoot = node;
	node = node->right;
	return lastRoot;

}

template<class T>
void AVLTree<T>::deleteAll(AVLTree<T>::Node* node) noexcept
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

template<class T>
int AVLTree<T>::height(const AVLTree<T>::Node* node) const noexcept
{
	/*if (!node) return 0;
	int leftH = height(node->left);
	int rightH = height(node->right);
	return 1 + (leftH > rightH ? leftH : rightH);*/
	if (!node) return 0;
	return node->height;
}

template<class T>
typename AVLTree<T>::Node* AVLTree<T>::findNode(const T& val, AVLTree<T>::Node* node) const noexcept {
	if (!node) return nullptr;
	if (node->value == val) return node;
	return val < node->value ? findNode(val, node->left) : findNode(val, node->right);
}

template<class T>
typename AVLTree<T>::Node* AVLTree<T>::makeCopy(const AVLTree<T>::Node* current)
{
	if (!current) return nullptr;
	AVLTree<T>::Node* node = nullptr;
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
		deleteAll(node->left);
		delete node;
		size = 0;
		throw;
	}
	return node;
}

template<class T>
AVLTree<T>::AVLTree<T>(const AVLTree<T>& other)
{
	root = makeCopy(other.root);
	//if root=nullptr -> problem with allocation
}

template<class T>
AVLTree<T>::AVLTree<T>(AVLTree<T>&& other) noexcept
	:AVLTree<T>()
{
	std::swap(other.size, this->size);
	std::swap(other.root, this->root);
}

template<class T>
AVLTree<T>& AVLTree<T>::operator=(const AVLTree<T>& other)
{
	if (&other != this) {
		AVLTree<T>::Node* newRoot = makeCopy(other.root);
		deleteAll(root);
		size = other.getSize();
		root = newRoot;
	}
	return *this;
}

template<class T>
AVLTree<T>& AVLTree<T>::operator=(AVLTree<T>&& other) noexcept
{

	if (&other != this) {
		deleteAll(root);
		size = 0;
		std::swap(other.size, this->size);
		std::swap(other.root, this->root);
	}
	return *this;
}

template<class T>
AVLTree<T>::~AVLTree<T>() noexcept
{
	if (root) {
		deleteAll(root);
		size = 0;
		root = nullptr;
	}
}

template<class T>
size_t AVLTree<T>::getSize() const noexcept
{
	return size;
}

template<class T>
bool AVLTree<T>::remove(const T& key) noexcept
{
	auto oldSize = size;
	root = deleteNode(key, root);
	return oldSize > size;
}

template<class T>
bool AVLTree<T>::insert(const T& key) noexcept
{
	try {
		root = insertNode(key, root);
		++size;
	}
	catch (const std::logic_error&) { //repetition
		return false;
	}
	//memory allocation errors wont change the state of the tree 
	return true;
}

template<class T>
size_t AVLTree<T>::getHeight() const noexcept {
	return height(root);
}

template<class T>
AVLIterator<T> AVLIterator<T>::operator++()
{
	if (nodes.empty()) {
		return AVLIterator<T>(nullptr);
	}
	typename AVLTree<T>::Node* node = nodes.top();
	nodes.pop();
	if (node->right) nodes.push(node->right);
	if (node->left) nodes.push(node->left);
	return *this;
}

template<class T>
AVLIterator<T>::AVLIterator(typename AVLTree<T>::Node* firstNode) noexcept
{
	if (!firstNode) return;
	nodes.push(firstNode);

}

template<class T>
const typename AVLTree<T>::Node* AVLIterator<T>::operator*() const
{
	if (nodes.empty()) return nullptr;
	return nodes.top();
}

template<class T>
bool AVLIterator<T>::operator==(const AVLIterator<T>& other) const noexcept {
	return operator*() == *other;
}

template<class T>
bool AVLIterator<T>::operator!=(const AVLIterator<T>& other) const noexcept {
	return operator*() != *other;
}