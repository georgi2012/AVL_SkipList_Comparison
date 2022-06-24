#pragma once
#include <iostream>


struct Node
{
	static const int UNSET = (4 << (sizeof(int) * 8 - 3));
	int value;
	const int lvl;
	// Array to hold pointers to node of different level
	Node** forward;
	Node(int _value , int _lvl )
		:value(_value), forward(new Node* [_lvl + 1]),lvl(_lvl)
	{
		for (int i = 0; i < _lvl; i++) {
			forward[i] = nullptr;
		}
	};
	~Node() noexcept
	{
		delete[] forward;
		value = UNSET;
	}

};

// Class for Skip list
class SkipList
{
private:
	//data
	// pointer to header node
	Node* first=nullptr;
	// Maximum level for this skip list
	int MAXLVL;
	// P is the fraction of the nodes with level
	// i pointers also having level i+1 pointers
	double P;
	// current level of skip list
	int lvl=0;

	//private methods
	int randomLevel() const noexcept;
	void clearAll() noexcept;
	Node* findNode(Node* start, int value) const noexcept;
	
public:
	//constructors and operators
	SkipList(const int maxLvl, const double fraction);
	SkipList(const SkipList& other);
	SkipList(SkipList&& other) noexcept;
	SkipList& operator=(const SkipList& other);
	SkipList& operator=(SkipList&& other) noexcept;
	~SkipList() noexcept;
	//Public methods
	bool insertElement(int val) noexcept;
	bool deleteElement(int val) noexcept;
	bool exists(int val) const noexcept;
	void displayList() const noexcept;
	void clearData() noexcept;
};