#include "SkipList.h"

SkipList::SkipList(const size_t maxLvl, const double _fraction)
	:first(new Node(Node::HEAD_VAL, maxLvl)), MAXLVL(maxLvl), fraction(_fraction), lvl(0) {}
//first node is set as starting only and his value is not used

SkipList::SkipList(SkipList&& other) noexcept
	:MAXLVL(other.MAXLVL), fraction(other.fraction), lvl(other.lvl), first(other.first), size(other.size)
{
	other.first = nullptr;
	other.lvl = 0;
	other.size = 0;
}

SkipList& SkipList::operator=(const SkipList& other)
{
	if (&other != this) {
		SkipList newList(other);
		*this = std::move(newList);
	}
	return *this;
}

SkipList& SkipList::operator=(SkipList&& other) noexcept
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

SkipList::SkipList(const SkipList& other)
{
	first = new Node(other.first->value, other.first->lvl); // ok to throw
	//copying pointers on different lvls
	Node* otherNode = other.first;
	Node* ourNode = first;
	//create all nodes on lowerst lvl
	while (otherNode->lvlNodes[0]) {
		otherNode = otherNode->lvlNodes[0]; // move to next
		try {
			ourNode->lvlNodes[0] = new Node(otherNode->value, otherNode->lvl); //create copy
		}
		catch (...) {
			clearAll();
			throw;
		}
		ourNode = ourNode->lvlNodes[0]; // move our to next
	}
	//now we have lvl 0, need pointers for other lvls
	for (int i = 1; i < other.lvl; i++) {
		otherNode = other.first;
		ourNode = first;
		while (otherNode->lvlNodes[i]) { //set all pointers for this lvl
			otherNode = otherNode->lvlNodes[i];//move
			ourNode->lvlNodes[i] = findNode(first->lvlNodes[0], otherNode->value); //find the right pointer that we have created on lvl 0
			ourNode = ourNode->lvlNodes[i];//move
		}
	}
	//
	MAXLVL = other.MAXLVL;
	lvl = other.lvl;
	fraction = other.fraction;
	size = other.size;
}

// create random level for node
size_t SkipList::randomLevel() const noexcept
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
void SkipList::clearAll() noexcept
{
	if (!first || !first->lvlNodes[0]) return;
	Node* prev = first->lvlNodes[0];
	Node* cur = prev->lvlNodes[0];

	while (cur) {
		delete prev;
		prev = cur;
		cur = cur->lvlNodes[0];
	}
	delete prev;
	for (int i = 0; i < MAXLVL; i++) {
		first->lvlNodes[i] = nullptr;
	}
	size = 0;
	lvl = 0;
}

Node* SkipList::findNode(Node* start, int value) const noexcept
{
	if (!start || start->value == value) return start;
	while (start->lvlNodes[0]) {
		start = start->lvlNodes[0];
		if (start->value == value) return start;
	}
	return nullptr;
}



SkipList::~SkipList() noexcept
{
	clearAll();
	delete first;
	first = nullptr;
}

// Insert given key in skip list
bool SkipList::insert(int val) noexcept
{
	if (!first) {
		try {
			first = new Node(val, MAXLVL);
		}
		catch (...) {
			return false;
		}
	}
	if (exists(val)) {
		return false; //no duplicates allowed
	}
	Node* cur = first;
	// create update array and initialize it
	Node** update;
	try {
		update = new Node * [MAXLVL + 1];
	}
	catch (...) {
		return false;
	}
	for (int i = 0; i < MAXLVL + 1; i++) {
		update[i] = nullptr;
	}

	/*    start from highest level of skip list
		move the current pointer lvlNodes while key
		is greater than key of node next to current
		Otherwise inserted current in update and
		move one level down and continue search
	*/
	for (int i = lvl; i >= 0; i--)
	{
		while (cur->lvlNodes[i] && cur->lvlNodes[i]->value < val)
		{
			cur = cur->lvlNodes[i];
		}
		update[i] = cur;
	}

	/* reached level 0 and lvlNodes pointer to
	   right, which is desired position to
	   insert key.
	*/
	cur = cur->lvlNodes[0];

	/* if current is NULL that means we have reached
	   to end of the level or current's key is not equal
	   to key to insert that means we have to insert
	   node between update[0] and current node */
	if (!cur || cur->value != val)
	{
		// Generate a random level for node
		int rlevel = randomLevel();

		/* If random level is greater than list's current
		   level (node with highest level inserted in
		   list so far), initialize update value with pointer
		   to header for further use */
		if (rlevel > lvl)
		{
			for (int i = lvl + 1; i < rlevel + 1; i++)
				update[i] = first;

			// Update the list current level
			lvl = rlevel;
		}

		// create new node with random level generated

		Node* n;
		try {
			n = new Node(val, rlevel);
		}
		catch (...) {
			delete[] update;
			return false;
		}
		// insert node by rearranging pointers
		for (int i = 0; i <= rlevel; i++)
		{
			n->lvlNodes[i] = update[i]->lvlNodes[i];
			update[i]->lvlNodes[i] = n;
		}
		//cout << "Successfully Inserted key " << key << "\n";
		++size;
		delete[] update;
		return true;
	}
	delete[] update;
}

// Delete element from skip list 
bool SkipList::remove(int val) noexcept
{
	/*if (size == 1 && first->value == val) {
		--size;
		delete first;
		first = nullptr;
		lvl = 0;
		return true;
	}*/
	Node* current = first;

	// create update array and initialize it
	Node** update;
	try {
		update = new Node * [MAXLVL + 1];
	}
	catch (...) {
		return false;
	}
	for (int i = 0; i < MAXLVL + 1; i++) {
		update[i] = nullptr;
	}

	/*    start from highest level of skip list
		move the current pointer lvlNodes while key
		is greater than key of node next to current
		Otherwise inserted current in update and
		move one level down and continue search
	*/
	for (int i = lvl; i >= 0; i--)
	{
		while (current->lvlNodes[i] && current->lvlNodes[i]->value < val)
		{
			current = current->lvlNodes[i];
		}
		update[i] = current;
	}

	/* reached level 0 and lvlNodes pointer to
	   right, which is possibly our desired node.*/

	current = current->lvlNodes[0];

	// If current node is target node
	if (current && current->value == val)
	{
		/* start from lowest level and rearrange
		   pointers just like we do in singly linked list
		   to remove target node */
		for (int i = 0; i <= lvl; i++)
		{
			/* If at level i, next node is not target
			   node, break the loop, no need to move
			  further level */
			if (update[i]->lvlNodes[i] != current)
				break;

			update[i]->lvlNodes[i] = current->lvlNodes[i];
		}

		// Remove levels having no elements
		while (lvl > 0 && !first->lvlNodes[lvl])
		{
			--lvl;
		}
		//cout << "Successfully deleted key " << key << "\n";
		delete[] update;
		--size;
		return true;
	}
	delete[] update;
}

bool SkipList::exists(int val) const noexcept
{
	return findNode(first, val) != nullptr;

}
size_t SkipList::getSize() const noexcept
{
	return size;
}


void SkipList::clearData() noexcept
{
	clearAll();
}


SListIterator SkipList::begin() const noexcept
{
	if (!first) return SListIterator(nullptr);
	return SListIterator(first->lvlNodes[0]);
}

SListIterator SkipList::end() const noexcept
{
	return SListIterator(nullptr);
}

//iter
SListIterator SListIterator::operator++()
{

	if (current) current = current->lvlNodes[0];
	return *this;
}

SListIterator::SListIterator(Node* firstNode) noexcept
	:current(firstNode) {}

const Node* SListIterator::operator*() const
{
	return current;
}


bool SListIterator::operator==(const SListIterator& other) const noexcept {
	return operator*() == *other;
}

bool SListIterator::operator!=(const SListIterator& other) const noexcept {
	return operator*() != *other;
}