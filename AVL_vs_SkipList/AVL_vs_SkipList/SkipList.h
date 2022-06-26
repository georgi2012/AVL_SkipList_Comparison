#pragma once
#include <iostream>

class SListIterator;
/// @brief Skip list's SLNode 
struct SLNode
{
	/// @brief Control value of MIN_INT to separate the value of the list's header SLNode
	static const int HEAD_VAL = (4 << (sizeof(int) * 8 - 3));
	/// @brief SLNode's given value
	int value;
	/// @brief The lvl of the SLNode - number of pointers that it has
	const int lvl;
	/// @brief Array to hold pointers to SLNodes of different levels
	SLNode** lvlSLNodes;

	/// @brief Constructor to set the value and lvl
	SLNode(int _value , int _lvl )
		:value(_value), lvlSLNodes(new SLNode* [_lvl + 1]),lvl(_lvl)
	{
		//All lvls are set to nullptr at first
		for (int i = 0; i < _lvl+1; i++) {
			lvlSLNodes[i] = nullptr;
		}
	}

	///@brief Constructor to delete the SLNodes data
	~SLNode() noexcept
	{
		delete[] lvlSLNodes;
		lvlSLNodes = nullptr;
	}

};

/// @brief SkipList class with no repeating elements allowed
class SkipList
{
private:
	//data
	/// @brief Pointer to the first header SLNode
	SLNode* first=nullptr;

	/// @brief Maximum level that the list can have
	size_t MAXLVL;

	/// @brief Fraction of the SLNodes with level X pointers that also have next level pointers.
	/// (1-fr) elements will be on lvl 1, (1-fr)^2 on lvl 2 and so on.
	double fraction;

	/// @brief Current highest lvl on a SLNode in the list
	size_t lvl=0;

	/// @brief Number of SLNodes (without the header) that are inserted in the list.
	size_t size = 0;

	//private methods
	/// @brief Returns a random integer value that is less than the MAXLVL
	size_t randomLevel() const noexcept;
	/// @brief Deletes all SLNodes. Sets size and lvl to 0. Do not delete the header SLNode
	void clearAll() noexcept;
	/// @brief Searches for a SLNode and returns it.
	/// @param start The header pointer.
	/// @param value Searched value.
	SLNode* findSLNode(SLNode* start, int value) const noexcept;
	
public:
	friend class SListIterator;
	//constructors and operators
	/// @brief Constructor to create a list with specific MAXLVL and fraction.
	SkipList(const size_t maxLvl, const double fraction);
	/// @brief Copy constructor. Makes a copy and sets it as a header.
	/// @param other SkipList to be copied. No changes will be made on it.
	SkipList(const SkipList& other);
	/// @brief Move contrustor. Takes other's header and data.
	/// @param other SkipList to be moves. All param will be nullified.
	SkipList(SkipList&& other) noexcept;
	/// @brief Makes a SkipList and returns it.
	/// @param other SkipList to be copied.
	SkipList& operator=(const SkipList& other);
	/// @brief Moves a giving SkipList by taking it's data.
	/// @param other SkipList to be moved. Data will be nullified.
	SkipList& operator=(SkipList&& other) noexcept;
	/// @brief Destructor to delete all data. Uses clearAll
	~SkipList() noexcept;
	//Public methods
	/// @brief Creates a new SLNode with a random lvl and places it in sorted order.
	/// @param val Value to be given to the new SLNode. No repetitions allowed.
	/// @return True if SLNode was created and inserted. Else false
	bool insert(int val) noexcept;
	/// @brief Removes a specific SLNode with given value if found.
	/// @param val Value to be removed
	/// @return True If SLNode was found and removed.
	bool remove(int val) noexcept;
	/// @brief Returns If a SLNode with given value exists in the list.
	/// @param val Searched value
	bool exists(int val) const noexcept;
	/// @brief Number of currently inserted SLNodes in the tree
	size_t getSize() const noexcept;
	/// @brief Method to delete all inserted SLNodes. Uses clearAll.
	void clearData() noexcept;
	//iteration
	/// @brief Returns iterator to the start (head) of the list
	SListIterator begin() const noexcept;
	/// @brief Returns iterator to the end (nullptr) of the tree
	SListIterator end() const noexcept;
};

/// @brief Skip List iterator that goes through lvl 0 elements.
class SListIterator {
private:
	//data
	/// @brief Current SLNode in the list.
	SLNode* current=nullptr;
	//methods
	/// @brief Constructor that sets the SLNode as current
	SListIterator(SLNode* head) noexcept;
public:
	friend class SkipList;
	//methods
	/// @brief Operator to move the stack to the next SLNode in the order.
	SListIterator  operator++();
	/// @brief Operator to get the next SLNode in the order
	const SLNode* operator*() const;
	/// @brief Operator to check if two Iterators are the same
	bool operator==(const SListIterator& other) const noexcept;
	/// @brief Operator to check if two Iterators are not the same
	bool operator!=(const SListIterator& other) const noexcept;

};