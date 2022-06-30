#include "SkipList.h"

SkipList::SkipList(const size_t maxLvl, const double _fraction)
	:first(new SLNode(SLNode::HEAD_VAL, maxLvl)), MAXLVL(maxLvl), fraction(_fraction), lvl(0) {}
//first SLNode is set as starting only and his value is not used

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

// create random level for SLNode
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

SLNode* SkipList::findSLNode(SLNode* start, int value) const noexcept
{
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



SkipList::~SkipList() noexcept
{
	clearAll();
	delete first;
	first = nullptr;
}

bool SkipList::insert(int val) noexcept
{
	if (!first) {
		try {
			first = new SLNode(SLNode::HEAD_VAL, MAXLVL);
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

// Delete element from skip list 
bool SkipList::remove(int val) noexcept
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

bool SkipList::exists(int val) const noexcept
{
	return findSLNode(first, val) != nullptr;

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
	return SListIterator(first->lvlSLNodes[0]);
}

SListIterator SkipList::end() const noexcept
{
	return SListIterator(nullptr);
}

size_t SkipList::getBytesUsed() const noexcept
{
	size_t nodesBytes = 0;
	SLNode* cur = first;
	while (cur) {
		nodesBytes += cur->getBytesUsed();
		cur = cur->lvlSLNodes[0];
	}
	return sizeof(SkipList) + nodesBytes + sizeof(SLNode::HEAD_VAL);
}

//iter
SListIterator SListIterator::operator++()
{

	if (current) current = current->lvlSLNodes[0];
	return *this;
}

SListIterator::SListIterator(SLNode* firstSLNode) noexcept
	:current(firstSLNode) {}

const SLNode* SListIterator::operator*() const
{
	return current;
}


bool SListIterator::operator==(const SListIterator& other) const noexcept {
	return operator*() == *other;
}

bool SListIterator::operator!=(const SListIterator& other) const noexcept {
	return operator*() != *other;
}