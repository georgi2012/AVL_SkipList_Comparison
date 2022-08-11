#include "T_AVLTree.h"
#include "T_SkipList.h"
#include <chrono>
//#include <unordered_set>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <cmath>
#include <string>
#include <algorithm> 
#include <random>

using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::nanoseconds;
using std::cout;
using std::string;
using std::to_string;

//SkipList<int>
int getOptimalLvlNum(int expectedSize, double p = 0.5) {
	return log2(expectedSize) / log2(1 / p) + 1; //log(p^-1)N
}

#define SLIST_IND 0
#define AVL_IND 1

struct TestHelperContainer {
	struct TestHelper {
		double insertion[2] = { 0 }, deletion[2] = { 0 }, search[2] = { 0 }, memory[2] = { 0 };
	} avg, slowest;
};

#pragma optimize( "", off )
TestHelperContainer findAvgInsertDelFind(const unsigned elemCnt, const int testsCnt = 30) {
	TestHelperContainer data;
	//std::unordered_set<int> set;
	int* arr = new int[elemCnt];
	//
	for (int i = 0; i < elemCnt; i++) {
		arr[i] = i;
	}
	//
	int value;
	const int intMax = ~(1 << (sizeof(int) * 8 - 1));
	SkipList<int> list(getOptimalLvlNum(elemCnt), 0.5);
	AVLTree<int> tree;

	//
	for (int j = 0; j < testsCnt; j++) {
		//std::cout << j;
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::shuffle(arr, arr + elemCnt, std::default_random_engine(seed));
		//insert
		auto start = steady_clock::now();
		auto end = steady_clock::now();
		for (int i = 0; i < elemCnt; i++) {
			start = steady_clock::now();
			list.insert(arr[i]);
			end = steady_clock::now();
			data.avg.insertion[SLIST_IND] += duration_cast<nanoseconds>(end - start).count();
			if (duration_cast<nanoseconds>(end - start).count() >
				data.slowest.insertion[SLIST_IND])
				data.slowest.insertion[SLIST_IND] = duration_cast<nanoseconds>(end - start).count();
		}

		//data.insertion[SLIST_IND] += duration_cast<nanoseconds>(end - start).count();
		data.avg.memory[SLIST_IND] += list.getBytesUsed();
		if (list.getBytesUsed() > data.slowest.memory[SLIST_IND]) {
			data.slowest.memory[SLIST_IND] = list.getBytesUsed();
		}
		//AVL insert


		for (int i = 0; i < elemCnt; i++) {
			start = steady_clock::now();
			tree.insert(arr[i]);
			end = steady_clock::now();
			data.avg.insertion[AVL_IND] += duration_cast<nanoseconds>(end - start).count();
			if (duration_cast<nanoseconds>(end - start).count() >
				data.slowest.insertion[AVL_IND])
				data.slowest.insertion[AVL_IND] = duration_cast<nanoseconds>(end - start).count();
		}
		//

		data.avg.memory[AVL_IND] += tree.getBytesUsed();
		if (tree.getBytesUsed() > data.slowest.memory[AVL_IND]) {
			data.slowest.memory[AVL_IND] = tree.getBytesUsed();
		}
		//List find
		for (int i = 0; i < elemCnt; i++) {
			start = steady_clock::now();
			list.exists(arr[i]);

			end = steady_clock::now();
			data.avg.search[SLIST_IND] += duration_cast<nanoseconds>(end - start).count();
			if (duration_cast<nanoseconds>(end - start).count() >
				data.slowest.search[SLIST_IND])
				data.slowest.search[SLIST_IND] = duration_cast<nanoseconds>(end - start).count();
		}
		//std::cout << "Height :" <<tree.getHeight() << std::endl;
		//tree find
		for (int i = 0; i < elemCnt; i++) {
			start = steady_clock::now();

			tree.exists(arr[i]);

			end = steady_clock::now();
			data.avg.search[AVL_IND] += duration_cast<nanoseconds>(end - start).count();
			if (duration_cast<nanoseconds>(end - start).count() >
				data.slowest.search[AVL_IND])
				data.slowest.search[AVL_IND] = duration_cast<nanoseconds>(end - start).count();
		}
		//List delete
		for (int i = 0; i < elemCnt; i++) {
			start = steady_clock::now();
			list.remove(arr[i]);

			end = steady_clock::now();
			data.avg.deletion[SLIST_IND] += duration_cast<nanoseconds>(end - start).count();
			if (duration_cast<nanoseconds>(end - start).count() >
				data.slowest.deletion[SLIST_IND])
				data.slowest.deletion[SLIST_IND] = duration_cast<nanoseconds>(end - start).count();
		}
		//tree delete

		for (int i = 0; i < elemCnt; i++) {
			start = steady_clock::now();

			tree.remove(arr[i]);

			end = steady_clock::now();
			data.avg.deletion[AVL_IND] += duration_cast<nanoseconds>(end - start).count();
			if (duration_cast<nanoseconds>(end - start).count() >
				data.slowest.deletion[AVL_IND])
				data.slowest.deletion[AVL_IND] = duration_cast<nanoseconds>(end - start).count();
		}
		//clear data
		tree.clearData();
		list.clearData();
		//set.clear();
	}
	delete[] arr;
	for (int i = 0; i < 2; i++) {
		data.avg.insertion[i] /= (testsCnt * elemCnt);
		data.avg.deletion[i] /= (testsCnt * elemCnt);
		data.avg.search[i] /= (testsCnt * elemCnt);
		data.avg.memory[i] /= (testsCnt);// *elemCnt);
	}

	return data;
}

#pragma optimize( "", off )
TestHelperContainer findAvgWhenWorkingWithManyElements(const unsigned elemCnt, const int testsCnt = 30)
{
	TestHelperContainer data;
	
	int* arr = new int[elemCnt];
	//
	for (int i = 0; i < elemCnt; i++) {
		arr[i] = i;
	}
	//
	int value;

	SkipList<int> list(getOptimalLvlNum(elemCnt), 0.5);
	AVLTree<int> tree;
	size_t cnt = 0;
	//
	for (int j = 0; j < testsCnt; j++) {
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::shuffle(arr, arr + elemCnt, std::default_random_engine(seed));
		//insert
		auto start = steady_clock::now();
		auto end = steady_clock::now();
		for (int i = 0; i < elemCnt; i++) {
			list.insert(arr[i]);
		}

		for (int i = 0; i < elemCnt; i++) {
			tree.insert(arr[i]);
		}

		//now we use operation while we stay with many elements in the structures
		for (int i2 = 0; i2 < 15; i2++) {

			int cntOfEl = (2 + rand() % 10) * (elemCnt / 100);
			cnt += cntOfEl;
			//now remove them
			for (int i = 0; i < cntOfEl; i++) {
				start = steady_clock::now();
				list.remove(arr[i]);
				end = steady_clock::now();
				data.avg.deletion[SLIST_IND] += duration_cast<nanoseconds>(end - start).count();
				if (duration_cast<nanoseconds>(end - start).count() >
					data.slowest.deletion[SLIST_IND])
					data.slowest.deletion[SLIST_IND] = duration_cast<nanoseconds>(end - start).count();
			}
			//avl
			for (int i = 0; i < cntOfEl; i++) {
				start = steady_clock::now();
				tree.remove(arr[i]);
				end = steady_clock::now();
				data.avg.deletion[AVL_IND] += duration_cast<nanoseconds>(end - start).count();
				if (duration_cast<nanoseconds>(end - start).count() >
					data.slowest.deletion[AVL_IND])
					data.slowest.deletion[AVL_IND] = duration_cast<nanoseconds>(end - start).count();
			}
			//and and add again
			for (int i = 0; i < cntOfEl; i++) {
				start = steady_clock::now();
				list.insert(arr[i]);
				end = steady_clock::now();
				data.avg.insertion[SLIST_IND] += duration_cast<nanoseconds>(end - start).count();
				if (duration_cast<nanoseconds>(end - start).count() >
					data.slowest.insertion[SLIST_IND])
					data.slowest.insertion[SLIST_IND] = duration_cast<nanoseconds>(end - start).count();
			}

			//AVL insert
			for (int i = 0; i < cntOfEl; i++) {
				start = steady_clock::now();
				tree.insert(arr[i]);
				end = steady_clock::now();
				data.avg.insertion[AVL_IND] += duration_cast<nanoseconds>(end - start).count();
				if (duration_cast<nanoseconds>(end - start).count() >
					data.slowest.insertion[AVL_IND])
					data.slowest.insertion[AVL_IND] = duration_cast<nanoseconds>(end - start).count();
			}
			//
			//List find
			for (int i = 0; i < cntOfEl; i++) {
				start = steady_clock::now();
				list.exists(arr[i]);

				end = steady_clock::now();
				data.avg.search[SLIST_IND] += duration_cast<nanoseconds>(end - start).count();
				if (duration_cast<nanoseconds>(end - start).count() >
					data.slowest.search[SLIST_IND])
					data.slowest.search[SLIST_IND] = duration_cast<nanoseconds>(end - start).count();
			}
			//tree find
			for (int i = 0; i < cntOfEl; i++) {
				start = steady_clock::now();

				tree.exists(arr[i]);

				end = steady_clock::now();
				data.avg.search[AVL_IND] += duration_cast<nanoseconds>(end - start).count();
				if (duration_cast<nanoseconds>(end - start).count() >
					data.slowest.search[AVL_IND])
					data.slowest.search[AVL_IND] = duration_cast<nanoseconds>(end - start).count();
			}
			data.avg.memory[AVL_IND] += tree.getBytesUsed();
			if (tree.getBytesUsed() > data.slowest.memory[AVL_IND]) {
				data.slowest.memory[AVL_IND] = tree.getBytesUsed();
			}
			data.avg.memory[SLIST_IND] += list.getBytesUsed();
			if (list.getBytesUsed() > data.slowest.memory[SLIST_IND]) {
				data.slowest.memory[SLIST_IND] = list.getBytesUsed();
			}
		}

		//List find
		tree.clearData();
		list.clearData();
		//set.clear();
	}
	delete[] arr;
	for (int i = 0; i < 2; i++) {
		data.avg.insertion[i] /= cnt;//(testsCnt * cnt);
		data.avg.deletion[i] /= cnt;
		data.avg.search[i] /= cnt;
		data.avg.memory[i] /= (testsCnt);// *elemCnt);
	}

	return data;
}

void printPrettyTable(TestHelperContainer::TestHelper& data, const string starter = "__________") {
	string avlData[] = { std::to_string((int)data.insertion[AVL_IND]) ,
					   std::to_string((int)data.deletion[AVL_IND]) ,
					   std::to_string((int)data.search[AVL_IND]),
					   std::to_string((int)data.memory[AVL_IND]) };
	string slData[] = { std::to_string((int)data.insertion[SLIST_IND]) ,
					   std::to_string((int)data.deletion[SLIST_IND]) ,
					   std::to_string((int)data.search[SLIST_IND]),
					   std::to_string((int)data.memory[SLIST_IND]) };
	const int firstColWidth = 10;//insertion,del,search -  "(in ns)"
	const int otherColsWidth = 10;
	string percentComp[] = { std::to_string(((int)data.insertion[AVL_IND]) * 100 / ((int)data.insertion[SLIST_IND])) ,
					  std::to_string(((int)data.deletion[AVL_IND]) * 100 / ((int)data.deletion[SLIST_IND])) ,
					  std::to_string(((int)data.search[AVL_IND]) * 100 / ((int)data.search[SLIST_IND])),
					  std::to_string(((int)data.memory[AVL_IND]) * 100 / ((int)data.memory[SLIST_IND])) };
	//row 1
	std::cout << "-----------------------------------------------\n";
	//string(firstColWidth, ' ')
	std::cout << starter << "|     AVL    |  SkipList  |  AVL/SL  |\n";
	//row 2
	std::cout << "Insertion |" << //first col
		std::string(otherColsWidth - avlData[0].size(), ' ') << avlData[0] << "ns|" << //second col
		std::string(otherColsWidth - slData[0].size(), ' ') << slData[0] << "ns|" << //third
		std::string(otherColsWidth - percentComp[0].size() - 1, ' ') << percentComp[0] << "%|" << std::endl; //forth
	//row 3
	std::cout << "Deletion  |" << //first col
		std::string(otherColsWidth - avlData[1].size(), ' ') << avlData[1] << "ns|" << //second col
		std::string(otherColsWidth - slData[1].size(), ' ') << slData[1] << "ns|" << //third
		std::string(otherColsWidth - percentComp[1].size() - 1, ' ') << percentComp[1] << "%|" << std::endl; //forth
	//row 3
	std::cout << "Search    |" << //first col
		std::string(otherColsWidth - avlData[2].size(), ' ') << avlData[2] << "ns|" << //second col
		std::string(otherColsWidth - slData[2].size(), ' ') << slData[2] << "ns|" << //third
		std::string(otherColsWidth - percentComp[2].size() - 1, ' ') << percentComp[2] << "%|" << std::endl; //forth
	std::cout << "Memory    |" << //first col
		std::string(otherColsWidth - avlData[3].size() + 1, ' ') << avlData[3] << "b|" << //second col
		std::string(otherColsWidth - slData[3].size() + 1, ' ') << slData[3] << "b|" << //third
		std::string(otherColsWidth - percentComp[3].size() - 1, ' ') << percentComp[3] << "%|" << std::endl; //forth
	std::cout << "-----------------------------------------------\n";
}



int main() {
	srand(time(NULL));
	try {
		const int testNum = 30;
		const int elemCnt = 1'000;
		std::cout << "Running " << testNum << " tests with " << elemCnt << " elements each and\n";
		std::cout << "taking their average results per operation...\n";
		auto data = findAvgInsertDelFind(elemCnt, testNum);
		
		std::cout << "\nResult:\n\n";
		std::cout << "\nAverage time that each operation needs to complete with one element.\n";
		printPrettyTable(data.avg);
		std::cout << "\n\nSlowest time for one operation.\n";
		//__________
		printPrettyTable(data.slowest, "__Slowest_");
		//
		std::cout << "\n\nAvg time when used constantly with many elements.\n";
		//__________
		data = findAvgWhenWorkingWithManyElements(elemCnt, testNum);
		printPrettyTable(data.avg);
	}
	catch (const std::bad_alloc&) {
		std::cout << "\nError :Not enough memory to perform the tests.\n";
	}
	catch (const std::exception& e) {
		std::cout << "\nError :" << e.what() << "\n";
	}


}

/*
Notes:
- SkipList has advantage when removing elements when working with less elements (~100'000)
- SkipList uses about 25% more memory
- AVL has overall better performance for single threaded processes
- SkipList is used more often for concurrent computing as when modifying 
or updating a node in list, you need to touch about 3 nodes - node itself and adjacent.
You dont need to lock and synchronized the whole structure.
However, when using AVL you should expect to go down and up the tree and lock many nodes
when balancing (rotating) and that makes dead lock and synchronization
problems more likely to occur.
 

 Note: Slowest time for operation depends on too many factors as processor business 
 at the moment of operation execution and is often not correct .

  Running 100 tests with 100 elements each and
Average time that each operation needs to complete with one element.
-----------------------------------------------
__________|     AVL    |  SkipList  |  AVL/SL  |
Insertion |       296ns|       341ns|       86%|
Deletion  |       230ns|       153ns|      150%|
Search    |        92ns|       136ns|       67%|
Memory    |       1608b|       2204b|       72%|
-----------------------------------------------

Slowest time for one operation.
-----------------------------------------------
__Slowest_|     AVL    |  SkipList  |  AVL/SL  |
Insertion |     38600ns|     21800ns|      177%|
Deletion  |     26100ns|     77900ns|       33%|
Search    |     21300ns|     36700ns|       58%|
Memory    |       1608b|       2408b|       66%|
-----------------------------------------------

  Running 50 tests with 1000 elements each
Result:
Average time that each operation needs to complete with one element.
-----------------------------------------------
__________|     AVL    |  SkipList  |  AVL/SL  |
Insertion |       441ns|       411ns|      107%|
Deletion  |       325ns|       221ns|      147%|
Search    |       135ns|       193ns|       69%|
Memory    |      16008b|      20177b|       79%|
-----------------------------------------------
Slowest time for one operation.
-----------------------------------------------
__Slowest_|     AVL    |  SkipList  |  AVL/SL  |
Insertion |   1027600ns|    129900ns|      791%|
Deletion  |     75400ns|    141600ns|       53%|
Search    |     28900ns|     29600ns|       97%|
Memory    |      16008b|      20572b|       77%|
-----------------------------------------------

--------
Running 30 tests with 100'000 elements
Result:
Average time that each operation needs to complete with one element.
-----------------------------------------------
__________|     AVL    |  SkipList  |  AVL/SL  |
Insertion |       623ns|       745ns|       83%|
Deletion  |       630ns|       519ns|      121%|
Search    |       275ns|       576ns|       47%|
Memory    |    1600008b|    2000408b|       79%|
-----------------------------------------------
Slowest time for one operation.
-----------------------------------------------
__Slowest_|     AVL    |  SkipList  |  AVL/SL  |
Insertion |    216600ns|   4367400ns|        4%|
Deletion  |   2408200ns|   9817200ns|       24%|
Search    |   1535300ns|   1042200ns|      147%|
Memory    |    1600008b|    2005116b|       79%|
-----------------------------------------------
Avg time when used constantly with many elements.
-----------------------------------------------
__________|     AVL    |  SkipList  |  AVL/SL  |
Insertion |       661ns|       851ns|       77%|
Deletion  |       659ns|       598ns|      110%|
Search    |       314ns|       597ns|       52%|
Memory    |   24000120b|   30002560b|      -63%|
-----------------------------------------------


Running 30 tests with 1'000'000 elements each 
Result:
Average time that each operation needs to complete with one element.
-----------------------------------------------
__________|     AVL    |  SkipList  |  AVL/SL  |
Insertion |      1005ns|      1331ns|       75%|
Deletion  |      1039ns|      1080ns|       96%|
Search    |       495ns|      1425ns|       34%|
Memory    |   16000008b|   20000570b|       79%|
-----------------------------------------------
Slowest time for one operation.
-----------------------------------------------
__Slowest_|     AVL    |  SkipList  |  AVL/SL  |
Insertion |   5410300ns|   1736900ns|      311%|
Deletion  |   2067600ns|    717600ns|      288%|
Search    |    174900ns|   1553300ns|       11%|
Memory    |   16000008b|   20015444b|       79%|
-----------------------------------------------
Avg time when used constantly with many elements.
-----------------------------------------------
__________|     AVL    |  SkipList  |  AVL/SL  |
Insertion |      1134ns|      1796ns|       63%|
Deletion  |      1132ns|      1510ns|       74%|
Search    |       588ns|      1470ns|       40%|
Memory    |  240000120b|  299996374b|       -5%|
-----------------------------------------------


Running 5 tests with 10'000'000 elements each and
taking their average results per operation...
Result:
Average time that each operation needs to complete with one element.
-----------------------------------------------
__________|     AVL    |  SkipList  |  AVL/SL  |
Insertion |      1734ns|      2391ns|       72%|
Deletion  |      1866ns|      2065ns|       90%|
Search    |       952ns|      2464ns|       38%|
Memory    |  160000008b|  199978320b|       -5%|
-----------------------------------------------
Slowest time for one operation.
-----------------------------------------------
__Slowest_|     AVL    |  SkipList  |  AVL/SL  |
Insertion |    338400ns|   3797700ns|        8%|
Deletion  |   4445900ns|    460400ns|      965%|
Search    |    989000ns|   1846200ns|       53%|
Memory    |  160000008b|  199978320b|       -5%|
-----------------------------------------------
Avg time when used constantly with many elements.
-----------------------------------------------
__________|     AVL    |  SkipList  |  AVL/SL  |
Insertion |      1909ns|      2883ns|       66%|
Deletion  |      1913ns|      2553ns|       74%|
Search    |      1062ns|      2526ns|       42%|
-----------------------------------------------
*/