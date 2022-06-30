#pragma once
#include <iostream>

template <class T>
class SListIterator;

/// @brief SkipList class with no repeating elements allowed
template <class T>
class SkipList
{
private:
	struct SLNode
	{
		/// @brief SLNode's given value
		T value;
		/// @brief The lvl of the SLNode - number of pointers that it has
		const int lvl;
		/// @brief Array to hold pointers to SLNodes of different levels
		SLNode** lvlSLNodes;

		/// @brief Constructor to set the value and lvl
		SLNode(const T& _value, int _lvl)
			:value(_value), lvlSLNodes(new SLNode* [_lvl + 1]), lvl(_lvl)
		{
			//All lvls are set to nullptr at first
			for (int i = 0; i < _lvl + 1; i++) {
				lvlSLNodes[i] = nullptr;
			}
		}

		///@brief Constructor to delete the SLNodes data
		~SLNode() noexcept
		{
			delete[] lvlSLNodes;
			lvlSLNodes = nullptr;
		}
		///@brief Returns the bytes used by this node atm
		size_t getBytesUsed() {
			return sizeof(value) + sizeof(lvl) + sizeof(SLNode*) * (lvl + 1);
		}

	};

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
	SLNode* findSLNode(SLNode* start, const T& value) const noexcept;
	
public:
	friend class SListIterator<T>;
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
	bool insert(const T& val) noexcept;
	/// @brief Removes a specific SLNode with given value if found.
	/// @param val Value to be removed
	/// @return True If SLNode was found and removed.
	bool remove(const T& val) noexcept;
	/// @brief Returns If a SLNode with given value exists in the list.
	/// @param val Searched value
	bool exists(const T& val) const noexcept;
	/// @brief Number of currently inserted SLNodes in the tree
	size_t getSize() const noexcept;
	/// @brief Method to delete all inserted SLNodes. Uses clearAll.
	void clearData() noexcept;
	//iteration
	/// @brief Returns iterator to the start (head) of the list
	SListIterator<T> begin() const noexcept;
	/// @brief Returns iterator to the end (nullptr) of the tree
	SListIterator<T> end() const noexcept;
	/// @brief Returns how many bytes are used by the structure atm
	size_t getBytesUsed() const noexcept;
};

/// @brief Skip List iterator that goes through lvl 0 elements.
template <class T>
class SListIterator {
private:
	//data
	/// @brief Current SLNode in the list.
	typename SkipList<T>::SLNode* current=nullptr;
	//methods
	/// @brief Constructor that sets the SLNode as current
	SListIterator(typename SkipList<T>::SLNode* head) noexcept;
public:
	friend class SkipList<T>;
	//methods
	/// @brief Operator to move the stack to the next SLNode in the order.
	SListIterator<T>  operator++();
	/// @brief Operator to get the next SLNode in the order
	const typename SkipList<T>::SLNode* operator*() const;
	/// @brief Operator to check if two Iterators are the same
	bool operator==(const SListIterator& other) const noexcept;
	/// @brief Operator to check if two Iterators are not the same
	bool operator!=(const SListIterator& other) const noexcept;

};


//impl

template <class T>
SkipList<T>::SkipList(const size_t maxLvl, const double _fraction)
	:first(new SLNode(T(), maxLvl)), MAXLVL(maxLvl), fraction(_fraction), lvl(0) {}
//first SLNode is set as starting only and his value is not used

template <class T>
SkipList<T>::SkipList<T>(SkipList<T>&& other) noexcept
	:MAXLVL(other.MAXLVL), fraction(other.fraction), lvl(other.lvl), first(other.first), size(other.size)
{
	other.first = nullptr;
	other.lvl = 0;
	other.size = 0;
}

template <class T>
SkipList<T>& SkipList<T>::operator=(const SkipList<T>& other)
{
	if (&other != this) {
		SkipList<T> newList(other);
		*this = std::move(newList);
	}
	return *this;
}

template <class T>
SkipList<T>& SkipList<T>::operator=(SkipList<T>&& other) noexcept
{
	if (&other != this) {
		clearAll();
		std::swap(other.size, size);
		std::swap(other.first, first);
		std::swap(other.fraction, fraction);
		std::swap(other.lvl, lvl);
		std::swap(other.MAXLVL, MAXLVL);
	}
	return *this;
}

template <class T>
SkipList<T>::SkipList<T>(const SkipList<T>& other)
{
	first = new SLNode(other.first->value, other.first->lvl); // ok to throw
	//copying pointers on different lvls
	SLNode* otherSLNode = other.first;
	SLNode* ourSLNode = first;
	//create all SLNodes on lowest lvl
	while (otherSLNode->lvlSLNodes[0]) {
		otherSLNode = otherSLNode->lvlSLNodes[0]; // move to next
		try {
			ourSLNode->lvlSLNodes[0] = new SLNode(otherSLNode->value, otherSLNode->lvl); //create copy
		}
		catch (...) {
			clearAll();
			throw;
		}
		ourSLNode = ourSLNode->lvlSLNodes[0]; // move our to next
	}
	//now we have lvl 0, need pointers for other lvls
	for (int i = 1; i < other.lvl; i++) {
		otherSLNode = other.first;
		ourSLNode = first;
		while (otherSLNode->lvlSLNodes[i]) { //set all pointers for this lvl
			otherSLNode = otherSLNode->lvlSLNodes[i];//move
			ourSLNode->lvlSLNodes[i] = findSLNode(first, otherSLNode->value); //find the right pointer that we have created on lvl 0
			ourSLNode = ourSLNode->lvlSLNodes[i];//move
		}
	}
	//
	MAXLVL = other.MAXLVL;
	lvl = other.lvl;
	fraction = other.fraction;
	size = other.size;
}


template <class T>
size_t SkipList<T>::randomLevel() const noexcept
{
	double r = (double)rand() / RAND_MAX;
	size_t randLvl = 0;
	while (r < fraction && randLvl < MAXLVL)
	{
		randLvl++;
		r = (double)rand() / RAND_MAX;
	}
	return randLvl;
}

template <class T>
void SkipList<T>::clearAll() noexcept
{
	if (!first || !first->lvlSLNodes[0]) return;
	SLNode* prev = first->lvlSLNodes[0];
	SLNode* cur = prev->lvlSLNodes[0];

	while (cur) {
		delete prev;
		prev = cur;
		cur = cur->lvlSLNodes[0];
	}
	delete prev;
	for (int i = 0; i <= MAXLVL; i++) {
		first->lvlSLNodes[i] = nullptr;
	}
	size = 0;
	lvl = 0;
}

template <class T>
typename SkipList<T>::SLNode* SkipList<T>::findSLNode(typename SkipList<T>::SLNode* start, const T& value) const noexcept
{
	if (start && start == first && start->value == value) {
		if (start->lvlSLNodes[0] && start->lvlSLNodes[0]->value == value) return start->lvlSLNodes[0];
		else return nullptr;
	}
	if (!start || start->value == value) return start;

	for (int i = lvl; i >= 0; i--) {
		while (start->lvlSLNodes[i] && start->lvlSLNodes[i]->value < value)
		{
			start = start->lvlSLNodes[i];
		}
	}
	//lvl 0 and maybe the wanted SLNode
	start = start->lvlSLNodes[0];
	return start && start->value == value ? start : nullptr;
}


template <class T>
SkipList<T>::~SkipList<T>() noexcept
{
	clearAll();
	delete first;
	first = nullptr;
}

template <class T>
bool SkipList<T>::insert(const T& val) noexcept
{
	if (!first) {
		try {
			first = new SLNode(T(), MAXLVL);
		}
		catch (...) {
			return false;
		}
	}
	if (exists(val)) {
		return false; //no duplicates allowed
	}
	SLNode* cur = first;
	// create update array and initialize it
	SLNode** update;
	try {
		update = new SLNode * [MAXLVL + 1];
	}
	catch (...) {
		return false;
	}
	for (int i = 0; i < MAXLVL + 1; i++) {
		update[i] = nullptr;
	}

	//start the search from highest lvl
	//update keeps the current value on higher SLNodes
	//when we move down
	for (int i = lvl; i >= 0; i--)
	{
		while (cur->lvlSLNodes[i] && cur->lvlSLNodes[i]->value < val)
		{
			cur = cur->lvlSLNodes[i];
		}
		update[i] = cur;
	}

	//lvl 0 and the next pointer should be the wanted place to insert
	cur = cur->lvlSLNodes[0];

	//if nullptr -> end of lvl and no dublicates
	// insert between update[0] and current
	if (!cur || cur->value != val)
	{
		int rlevel = randomLevel();

		//if lvl is higher than current ,init update value with pointer to header
		if (rlevel > lvl)
		{
			for (int i = lvl + 1; i < rlevel + 1; i++)
				update[i] = first;

			// change the current level
			lvl = rlevel;
		}

		// New SLNode with random level
		SLNode* n;
		try {
			n = new SLNode(val, rlevel);
		}
		catch (...) {
			delete[] update;
			return false;
		}
		// insert SLNode by rearranging pointers
		for (int i = 0; i <= rlevel; i++)
		{
			n->lvlSLNodes[i] = update[i]->lvlSLNodes[i];
			update[i]->lvlSLNodes[i] = n;
		}

		++size;
		delete[] update;
		return true;
	}
	delete[] update;
	return false;
}

template <class T>
bool SkipList<T>::remove(const T& val) noexcept
{
	SLNode* current = first;

	// create update array and initialize it
	SLNode** update;
	try {
		update = new SLNode * [MAXLVL + 1];
	}
	catch (...) {
		return false;
	}
	for (int i = 0; i < MAXLVL + 1; i++) {
		update[i] = nullptr;
	}

	//start the search from highest lvl
	//update keeps the current value on higher SLNodes
	//when we move down
	for (int i = lvl; i >= 0; i--)
	{
		while (current->lvlSLNodes[i] && current->lvlSLNodes[i]->value < val)
		{
			current = current->lvlSLNodes[i];
		}
		update[i] = current;
	}

	//reached lvl 0 and maybe thats the wanted SLNode
	current = current->lvlSLNodes[0];

	//if is searched SLNode
	if (current && current->value == val)
	{
		//starts from lowest and rearrange to remove the target
		for (int i = 0; i <= lvl; i++)
		{
			//if is not wanted SLNode on this lvl, more on next
			if (update[i]->lvlSLNodes[i] != current)
				break;
			//update
			update[i]->lvlSLNodes[i] = current->lvlSLNodes[i];
		}

		// Remove empty lvls
		while (lvl > 0 && !first->lvlSLNodes[lvl])
		{
			--lvl;
		}
		delete[] update;
		--size;
		return true;
	}
	delete[] update;
	return false; //not found
}

template <class T>
bool SkipList<T>::exists(const T& val) const noexcept
{
	return findSLNode(first, val) != nullptr;

}

template <class T>
size_t SkipList<T>::getSize() const noexcept
{
	return size;
}

template <class T>
void SkipList<T>::clearData() noexcept
{
	clearAll();
}

template <class T>
SListIterator<T> SkipList<T>::begin() const noexcept
{
	if (!first) return SListIterator<T>(nullptr);
	return SListIterator<T>(first->lvlSLNodes[0]);
}

template <class T>
SListIterator<T> SkipList<T>::end() const noexcept
{
	return SListIterator<T>(nullptr);
}

template <class T>
size_t SkipList<T>::getBytesUsed() const noexcept
{
	size_t nodesBytes = 0;
	SLNode* cur = first;
	while (cur) {
		nodesBytes += cur->getBytesUsed();
		cur = cur->lvlSLNodes[0];
	}
	return sizeof(SkipList<T>) + nodesBytes;//+ sizeof(SLNode::HEAD_VAL);
}

//iter
template <class T>
SListIterator<T> SListIterator<T>::operator++()
{

	if (current) current = current->lvlSLNodes[0];
	return *this;
}

template <class T>
SListIterator<T>::SListIterator(typename SkipList<T>::SLNode* firstSLNode) noexcept
	:current(firstSLNode) {}

template <class T>
const typename SkipList<T>::SLNode* SListIterator<T>::operator*() const
{
	return current;
}

template <class T>
bool SListIterator<T>::operator==(const SListIterator<T>& other) const noexcept {
	return operator*() == *other;
}

template <class T>
bool SListIterator<T>::operator!=(const SListIterator<T>& other) const noexcept {
	return operator*() != *other;
}