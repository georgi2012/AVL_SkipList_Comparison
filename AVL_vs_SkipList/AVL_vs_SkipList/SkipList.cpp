#include "SkipList.h"

SkipList::SkipList(const int maxLvl, const double fraction)
	:first(new Node(Node::UNSET, MAXLVL)), MAXLVL(maxLvl), P(fraction), lvl(0) {}

SkipList::SkipList(SkipList&& other) noexcept
	:MAXLVL(other.MAXLVL), P(other.P), lvl(other.lvl), first(other.first)
{
	other.first = nullptr;
	other.lvl = 0;
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
		std::swap(other.first, first);
		std::swap(other.P, P);
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
	while (otherNode->forward[0]) {
		otherNode = otherNode->forward[0]; // move to next
		try {
			ourNode->forward[0] = new Node(otherNode->value, otherNode->lvl); //create copy
		}
		catch (...) {
			clearAll();
			throw;
		}
		ourNode = ourNode->forward[0]; // move our to next
	}
	//now we have lvl 0, need pointers for other lvls
	//otherNode = other.first;
	//ourNode = first;
	for (int i = 1; i < other.lvl; i++) {
		otherNode = other.first;
		ourNode = first;
		while (otherNode->forward[i]) { //set all pointers for this lvl
			otherNode = otherNode->forward[i];//move
			ourNode->forward[i] = findNode(first->forward[0], otherNode->value); //find the right pointer that we have created on lvl 0
			ourNode = ourNode->forward[i];//move
		}
	}
	//
	MAXLVL = other.MAXLVL;
	lvl = other.lvl;
	P = other.P;
}

// create random level for node
int SkipList::randomLevel() const noexcept
{
	double r = (double)rand() / RAND_MAX;
	int randLvl = 0;
	while (r < P && randLvl < MAXLVL)
	{
		randLvl++;
		r = (double)rand() / RAND_MAX;
	}
	return randLvl;
}
void SkipList::clearAll() noexcept
{
	if (!first) return;
	Node* cur = first->forward[0];
	Node* prev = first;
	while (cur) {
		delete prev;
		prev = cur;
		cur = cur->forward[0];
	}
	delete prev;
	lvl = 0;
}

Node* SkipList::findNode(Node* start, int value) const noexcept
{
	if (start->value == value) return start;
	while (start->forward[0]) {
		start = start->forward[0];
		if (start->value == value) return start;
	}
	return nullptr;
}



SkipList::~SkipList() noexcept
{
	clearAll();
}

// Insert given key in skip list
bool SkipList::insertElement(int val) noexcept
{
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
		move the current pointer forward while key
		is greater than key of node next to current
		Otherwise inserted current in update and
		move one level down and continue search
	*/
	for (int i = lvl; i >= 0; i--)
	{
		while (cur->forward[i] != NULL && cur->forward[i]->value < val)
		{
			cur = cur->forward[i];
		}
		update[i] = cur;
	}

	/* reached level 0 and forward pointer to
	   right, which is desired position to
	   insert key.
	*/
	cur = cur->forward[0];

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
			n->forward[i] = update[i]->forward[i];
			update[i]->forward[i] = n;
		}
		//cout << "Successfully Inserted key " << key << "\n";
		delete[] update;
		return true;
	}
};

// Delete element from skip list 
bool SkipList::deleteElement(int val) noexcept
{
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
		move the current pointer forward while key
		is greater than key of node next to current
		Otherwise inserted current in update and
		move one level down and continue search
	*/
	for (int i = lvl; i >= 0; i--)
	{
		while (current->forward[i] && current->forward[i]->value < val)
		{
			current = current->forward[i];
		}
		update[i] = current;
	}

	/* reached level 0 and forward pointer to
	   right, which is possibly our desired node.*/
	current = current->forward[0];

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
			if (update[i]->forward[i] != current)
				break;

			update[i]->forward[i] = current->forward[i];
		}

		// Remove levels having no elements
		while (lvl > 0 && first->forward[lvl] == 0)
		{
			--lvl;
		}
		//cout << "Successfully deleted key " << key << "\n";
		delete[] update;
		return true;
	}
};

// Search for element in skip list
bool SkipList::exists(int val) const noexcept
{
	Node* current = first;

	/*    start from highest level of skip list
		move the current pointer forward while key
		is greater than key of node next to current
		Otherwise inserted current in update and
		move one level down and continue search
	*/
	for (int i = lvl; i >= 0; i--)
	{
		while (current->forward[i] && current->forward[i]->value < val)
		{
			current = current->forward[i];
		}

	}

	/* reached level 0 and advance pointer to
	   right, which is possibly our desired node*/
	current = current->forward[0];

	// If current node have key equal to
	// search key, we have found our target node
	return (current && current->value == val);

};

// Display skip list level wise
void SkipList::displayList() const noexcept
{
	std::cout << "\n*****Skip List*****" << "\n";
	for (int i = 0; i <= lvl; i++)
	{
		Node* node = first->forward[i];
		std::cout << "Level " << i << ": ";
		while (node != NULL)
		{
			std::cout << node->value << " ";
			node = node->forward[i];
		}
		std::cout << "\n";
	}
}
void SkipList::clearData() noexcept
{
	clearAll();
}

