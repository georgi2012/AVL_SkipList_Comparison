#pragma once
#include <iostream>
#include <stack>

//template?
class AVLIterator;

/// @brief Tree node structure that keeps value and children pointers
struct Node {
	/// @brief Node pointers for the children
	Node* left, * right;
	/// @brief Value of the node 
	int value;
	/// @brief Height of the subtree of that node 
	short height = 0;
	/// @brief Default constructor that sets value
	Node(const int value, Node* left = nullptr, Node* right = nullptr)
		: value(value), left(left), right(right) {}
};

/// @brief Self-balancing AVL tree with no repetitions rule
class AVLTree {
private:
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
	Node* findNode(const int val, Node* node) const noexcept;
	/// @brief Method to insert a node with specific value
	/// @param val Value to be added
	/// @param node Starting node for the adding
	/// @return Pointer to the last used node
	Node* insertNode(const int val, Node* node);
	/// @brief Method to remove a node with specific value
	/// @param val Value to be removed
	/// @param node Starting node for the adding
	/// @return Pointer to the last used node
	Node* deleteNode(const int val, Node* node) noexcept;
	/// @brief Method to balance the tree when inserted a new node
	/// @param val Value that has been changed lastly
	/// @param node Starting node for the balance
	/// @return Pointer to the last rotated node
	Node* balanceTreeInsertion(const int val, Node* node) noexcept;
	/// @brief Method to balance the tree when deleted a node
	/// @param val Value that has been changed lastly
	/// @param node Starting node for the balance
	/// @return Pointer to the last rotated node
	Node* balanceTreeDeletion(const int val, Node* node) noexcept;
	/// @brief Standart Right rotation for the specific node
	Node* rightRotate(Node* node) noexcept;
	/// @brief Standart Left rotation for the specific node
	Node* leftRotate(Node* node) noexcept;
public:
	friend class AVLIterator;
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
	bool insert(int key) noexcept;
	/// @brief Removes element through deleteNode() method with specific key. Returns if operation was successful
	bool remove(int key) noexcept;
	/// @brief Returns if a node with such key exists. Uses findNode()
	bool exists(int key) const noexcept;
	/// @brief Returns tree height
	size_t getHeight() const noexcept;
	/// @brief Deleted all nodes and sets size to 0
	void clearData() noexcept;
	//iteration
	/// @brief Returns iterator to the start (root) of the tree
	AVLIterator begin() const noexcept;
	/// @brief Returns iterator to the end (nullptr) of the tree
	AVLIterator end() const noexcept;
};

/// @brief AVL Tree iterator using left-parent-right traversal
class AVLIterator {
private:
	//data
	/// @brief Stack of nodes to keep the next node in the order
	std::stack<Node*> nodes;
	//methods
	/// @brief Constructor that pushes the given node as root of the traversal
	AVLIterator(Node* firstNode) noexcept;
public:
	friend class AVLTree;
	//methods
	/// @brief Operator to move the stack to the next node in the order.
	AVLIterator  operator++();
	/// @brief Operator to get the next Node in the order
	const Node* operator*() const;
	/// @brief Operator to check if two Iterators are the same
	bool operator==(const AVLIterator& other) const noexcept;
	/// @brief Operator to check if two Iterators are not the same
	bool operator!=(const AVLIterator& other) const noexcept;

};