#include "AVLTree.h"
#include "SkipList.h"
#include <chrono>
#include <unordered_set>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <cmath>
#include <string>

using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::nanoseconds;
using std::cout;
using std::string;
using std::to_string;
//chrono :
//20-100 times repeat , sum up, find srednoto , medianata ,(standartna deviaciq?)
//medianata - v obshtiq sluchai kolko otnema
//srednoto  - sredno vreme
//standartna deviaciq - sluchai koito e mn baven i se sluchva opredelen br puti

//SkipList
int getOptimalLvlNum(int expectedSize, double p = 0.5) {
	return log2(expectedSize) / log2(1 / p) + 1; //log(p^-1)N
}

#define SLIST_IND 0
#define AVL_IND 1

struct TestHelper {
	double insertion[2] = { 0 }, deletion[2] = { 0 }, search[2] = { 0 };
};


TestHelper findAvgInsertDelFindNotRand(const unsigned elemCnt, const int testsCnt = 30) {
	double avgFind = 0;
	double avgDel = 0;
	double avgInsert = 0;
	TestHelper data;
	//std::unordered_set<int> set;
	std::vector<int> set;
	int value;
	const int intMax = ~(1 << (sizeof(int) * 8 - 1));
	SkipList list(getOptimalLvlNum(elemCnt), 0.5);
	AVLTree tree;
	if (elemCnt > 20000) {
		std::cout << "(Due to the many elements choice, random number generation might be really slow.)\n";
	}
	for (int i = 0; i < testsCnt; i++) {
		set.resize(elemCnt);
		//make elements
		for (int i = 0; i < elemCnt; i++) {
			//do {
			//	value = rand() % intMax;// -intMax / 2;
			//} while (set.find(value) != set.end());//create unique elements
			//set.insert(value);
			set[i] = i;
		}
		//std::cout << "made";
		//insert
		auto start = steady_clock::now();
		for (auto entry : set) {
			if (!list.insert(entry))
				throw std::runtime_error("Problem inserting in List ");
		}
		auto end = steady_clock::now();
		data.insertion[SLIST_IND] += duration_cast<nanoseconds>(end - start).count();

		//AVL insert
		start = steady_clock::now();
		for (auto entry : set) {
			if (!tree.insert(entry))
				throw std::runtime_error("Problem inserting in tree ");
		}
		end = steady_clock::now();
		data.insertion[AVL_IND] += duration_cast<nanoseconds>(end - start).count();

		//List find
		start = steady_clock::now();
		for (auto entry : set) {
			if (!list.exists(entry))
				throw std::runtime_error("Problem searching in List ");
		}
		end = steady_clock::now();
		data.search[SLIST_IND] += duration_cast<nanoseconds>(end - start).count();

		//tree find
		start = steady_clock::now();
		for (auto entry : set) {
			if (!tree.exists(entry))
				throw std::runtime_error("Problem searching in tree ");
		}
		end = steady_clock::now();
		data.search[AVL_IND] += duration_cast<nanoseconds>(end - start).count();

		//List delete
		start = steady_clock::now();
		for (auto entry : set) {
			if (!list.remove(entry))
				throw std::runtime_error("Problem deleting in List ");
		}
		end = steady_clock::now();
		data.deletion[SLIST_IND] += duration_cast<nanoseconds>(end - start).count();

		//tree delete
		int cnt = -1;
		start = steady_clock::now();
		for (auto entry : set) {
			++cnt;
			if (!tree.remove(entry))
				//std::cout << "Problem in tree del - " << cnt <<"_"<<i<< std::endl;
				throw std::runtime_error("Problem deleting in tree ");
		}
		end = steady_clock::now();
		data.deletion[AVL_IND] += duration_cast<nanoseconds>(end - start).count();
		//clear data
		tree.clearData();
		list.clearData();
		set.clear();
	}

	for (int i = 0; i < 2; i++) {
		data.insertion[i] /= (testsCnt * elemCnt);
		data.deletion[i] /= (testsCnt * elemCnt);
		data.search[i] /= (testsCnt * elemCnt);
	}
	return data;
}


TestHelper findAvgInsertDelFind(const unsigned elemCnt, const int testsCnt = 30) {
	double avgFind = 0;
	double avgDel = 0;
	double avgInsert = 0;
	TestHelper data;
	std::unordered_set<int> set;
	int value;
	const int intMax = ~(1 << (sizeof(int) * 8 - 1));
	SkipList list(getOptimalLvlNum(elemCnt), 0.5);
	AVLTree tree;
	if (elemCnt > 20000) {
		std::cout << "(Due to the many elements choice, random number generation might be really slow.)\n";
	}
	for (int i = 0; i < testsCnt; i++) {
		//make elements
		for (int i = 0; i < elemCnt; i++) {
			do {
				value = rand() % intMax;// -intMax / 2;
			} while (set.find(value) != set.end());//create unique elements
			set.insert(value);
		}
		//std::cout << "made";
		//insert
		auto start = steady_clock::now();
		for (auto entry : set) {
			if (!list.insert(entry))
				throw std::runtime_error("Problem inserting in List ");
		}
		auto end = steady_clock::now();
		data.insertion[SLIST_IND] += duration_cast<nanoseconds>(end - start).count();

		//AVL insert
		start = steady_clock::now();
		for (auto entry : set) {
			if (!tree.insert(entry))
				throw std::runtime_error("Problem inserting in tree ");
		}
		end = steady_clock::now();
		data.insertion[AVL_IND] += duration_cast<nanoseconds>(end - start).count();

		//List find
		start = steady_clock::now();
		for (auto entry : set) {
			if (!list.exists(entry))
				throw std::runtime_error("Problem searching in List ");
		}
		end = steady_clock::now();
		data.search[SLIST_IND] += duration_cast<nanoseconds>(end - start).count();

		//tree find
		start = steady_clock::now();
		for (auto entry : set) {
			if (!tree.exists(entry))
				throw std::runtime_error("Problem searching in tree ");
		}
		end = steady_clock::now();
		data.search[AVL_IND] += duration_cast<nanoseconds>(end - start).count();

		//List delete
		start = steady_clock::now();
		for (auto entry : set) {
			if (!list.remove(entry))
				throw std::runtime_error("Problem deleting in List ");
		}
		end = steady_clock::now();
		data.deletion[SLIST_IND] += duration_cast<nanoseconds>(end - start).count();

		//tree delete
		int cnt = -1;
		start = steady_clock::now();
		for (auto entry : set) {
			++cnt;
			if (!tree.remove(entry))
				//std::cout << "Problem in tree del - " << cnt <<"_"<<i<< std::endl;
				throw std::runtime_error("Problem deleting in tree ");
		}
		end = steady_clock::now();
		data.deletion[AVL_IND] += duration_cast<nanoseconds>(end - start).count();
		//clear data
		tree.clearData();
		list.clearData();
		set.clear();
	}

	for (int i = 0; i < 2; i++) {
		data.insertion[i] /= (testsCnt * elemCnt);
		data.deletion[i] /= (testsCnt * elemCnt);
		data.search[i] /= (testsCnt * elemCnt);
	}
	return data;
}

void printPrettyTable(TestHelper& data) {
	string avlData[] = { std::to_string((int)data.insertion[AVL_IND]) ,
					   std::to_string((int)data.deletion[AVL_IND]) ,
					   std::to_string((int)data.search[AVL_IND]) };
	string slData[] = { std::to_string((int)data.insertion[SLIST_IND]) ,
					   std::to_string((int)data.deletion[SLIST_IND]) ,
					   std::to_string((int)data.search[SLIST_IND]) };
	const int firstColWidth = 10;//insertion,del,search -  "(in ns)"
	const int otherColsWidth = 10;
	string percentComp[]= { std::to_string(((int)data.insertion[AVL_IND])*100/ ((int)data.insertion[SLIST_IND])) ,
					  std::to_string(((int)data.deletion[AVL_IND]) * 100 / ((int)data.deletion[SLIST_IND])) ,
					  std::to_string(((int)data.search[AVL_IND]) * 100 / ((int)data.search[SLIST_IND])) };
	//row 1
	std::cout << string(firstColWidth,' ') + "|     AVL    |  SkipList  |  AVL/SL  |\n";
	//row 2
	std::cout << "Insertion |" << //first col
		std::string(otherColsWidth - avlData[0].size(),' ') << avlData[0] << "ns|" << //second col
		std::string(otherColsWidth - slData[0].size(), ' ') << slData[0] << "ns|" << //third
		std::string(otherColsWidth - percentComp[0].size()-1, ' ') << percentComp[0] << "%|" << std::endl; //forth
	//row 3
	std::cout << "Deletion  |" << //first col
		std::string(otherColsWidth - avlData[1].size(), ' ') << avlData[1] << "ns|" << //second col
		std::string(otherColsWidth - slData[1].size(), ' ') << slData[1] << "ns|" << //third
		std::string(otherColsWidth - percentComp[1].size()-1, ' ') << percentComp[1] << "%|" << std::endl; //forth
	//row 3
	std::cout << "Search    |" << //first col
		std::string(otherColsWidth - avlData[2].size(), ' ') << avlData[2] << "ns|" << //second col
		std::string(otherColsWidth - slData[2].size(), ' ') << slData[2] << "ns|" << //third
		std::string(otherColsWidth - percentComp[2].size()-1, ' ') << percentComp[2] << "%|" << std::endl; //forth

}


int main() {
	srand(time(NULL));
	try {
		const int testNum = 30;
		const int elemCnt = 10'000;
		std::cout << "Running "<<testNum<<" tests with "<<elemCnt<<" elements each and\n";
		std::cout << "taking their average results per operation...\n";
		TestHelper data = findAvgInsertDelFind(elemCnt,testNum);
		//TestHelper data = findAvgInsertDelFindNotRand(elemCnt, testNum);
		std::cout << "\nResult:\n\n";
	
		printPrettyTable(data);
		std::cout << "\nAverage time that each operation needs to complete with one element.\n";
	}
	catch (const std::exception& e) {
		std::cout << "\nError :" << e.what() << "\n";
	}


}

/*
Notes:
- SkipList's insertion is nearly always faster
- SkipList deletion is nearly 3 times faster as there is less balancing
- With many elements search in SkipList might be slightly slower
- With consequent numbers SkipList is way better than AVL

-With random numbers:
with 30'000
Result:

		  |     AVL    |  SkipList  |  AVL/SL  |
Insertion |      2364ns|      1535ns|      154%|
Deletion  |      2256ns|       792ns|      284%|
Search    |       488ns|       545ns|       89%|
---------------------------------------------------
with 20'000

		  |     AVL    |  SkipList  |  AVL/SL  |
Insertion |      2302ns|      1525ns|      150%|
Deletion  |      2196ns|       762ns|      288%|
Search    |       465ns|       509ns|       91%|
---------------------------------------------------
with 10'000
		  |     AVL    |  SkipList  |  AVL/SL  |
Insertion |      2156ns|      1402ns|      153%|
Deletion  |      1942ns|       683ns|      284%|
Search    |       419ns|       429ns|       97%|
---------------------------------------------------
with 5'000
		  |     AVL    |  SkipList  |  AVL/SL  |
Insertion |      2012ns|      1396ns|      144%|
Deletion  |      1813ns|       637ns|      284%|
Search    |       394ns|       385ns|      102%|
---------------------------------------------------
with 1'000
		  |     AVL    |  SkipList  |  AVL/SL  |
Insertion |      1608ns|      1188ns|      135%|
Deletion  |      1430ns|       530ns|      269%|
Search    |       291ns|       238ns|      122%|
---------------------------------------------------
with 100
		  |     AVL    |  SkipList  |  AVL/SL  |
Insertion |      1092ns|      1105ns|       98%|
Deletion  |       925ns|       466ns|      198%|
Search    |       219ns|       189ns|      115%|
---------------------------------------------------

Not randomized - consequent input
with 100'000
		  |     AVL    |  SkipList  |  AVL/SL  |
Insertion |      2411ns|      1148ns|      210%|
Deletion  |      1523ns|       411ns|      370%|
Search    |       317ns|       198ns|      160%|
*/