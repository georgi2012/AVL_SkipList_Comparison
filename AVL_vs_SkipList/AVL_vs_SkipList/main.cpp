#include "AVLTree.h"
#include "SkipList.h"
#include <chrono>
#include <unordered_set>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <cmath>

using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::cout;
//chrono :
//20-100 times repeat , sum up, find srednoto , medianata ,(standartna deviaciq?)
//medianata - v obshtiq sluchai kolko otnema
//srednoto  - sredno vreme
//standartna deviaciq - sluchai koito e mn baven i se sluchva opredelen br puti

//AVL
double testAVLInsertion(const unsigned elemCnt, const int testsCnt = 30) {
	double avarage = 0;
	std::unordered_set<int> set;
	int value;
	AVLTree tree;
	for (int i = 0; i < testsCnt; i++) {
		//make elements
		for (int i = 0; i < elemCnt; i++) {
			do {
				value = rand() % 100000 - 50000;
			} while (set.find(value) != set.end());//create unique elements
			set.insert(value);
		}
		//input
		int cnt = 0;
		auto start = steady_clock::now();
		for (auto entry : set) {
			if (!tree.insert(entry))
				throw std::runtime_error(cnt + " Problem inserting in tree ");
			cnt++;
		}
		auto end = steady_clock::now();
		avarage += duration_cast<milliseconds>(end - start).count();/// testsCnt;
		tree.clearData();
		set.clear();
	}
	return avarage / (testsCnt * elemCnt);
}
//
//SkipList
int getOptimalLvlNum(int expectedSize, double p = 0.5) {
	return log2(expectedSize) / log2(1 / p) + 1; //log(p^-1)N
}
double testSListInsertion(const unsigned elemCnt, const int testsCnt = 30) {
	double avarage = 0;
	std::unordered_set<int> set;
	int value;
	SkipList list(getOptimalLvlNum(elemCnt), 0.5);
	for (int i = 0; i < testsCnt; i++) {
		//make elements
		for (int i = 0; i < elemCnt; i++) {
			do {
				value = rand() % 100000 - 50000;
			} while (set.find(value) != set.end());//create unique elements
			set.insert(value);
		}
		//input
		int cnt = 0;
		auto start = steady_clock::now();
		for (auto entry : set) {
			if (!list.insert(entry))
				throw std::runtime_error("Problem inserting in List " + cnt);
			cnt++;
		}
		auto end = steady_clock::now();
		avarage += duration_cast<milliseconds>(end - start).count();/// testsCnt;
		list.clearData();
		set.clear();
	}
	return avarage / (testsCnt * elemCnt);
}

#define SLIST_IND 0
#define AVL_IND 1

struct TestHelper {
	double insertion[2] = { 0 }, deletion[2] = { 0 }, search[2] = { 0 };
};

TestHelper findAvgInsertDelFind(const unsigned elemCnt, const int testsCnt = 30) {
	double avgFind = 0;
	double avgDel=0;
	double avgInsert = 0;
	TestHelper data; 
	std::unordered_set<int> set;
	int value;
	SkipList list(getOptimalLvlNum(elemCnt), 0.5);
	AVLTree tree;
	for (int i = 0; i < testsCnt; i++) {
		//make elements
		for (int i = 0; i < elemCnt; i++) {
			do {
				value = rand() % 100000 - 50000;
			} while (set.find(value) != set.end());//create unique elements
			set.insert(value);
		}
		//insert
		auto start = steady_clock::now();
		for (auto entry : set) {
			if (!list.insert(entry))
				throw std::runtime_error("Problem inserting in List ");
		}
		auto end = steady_clock::now();
		data.insertion[SLIST_IND]+= duration_cast<milliseconds>(end - start).count();
		
		//AVL insert
		start = steady_clock::now();
		for (auto entry : set) {
			if (!tree.insert(entry))
				throw std::runtime_error("Problem inserting in tree ");
		}
		end = steady_clock::now();
		data.insertion[AVL_IND] += duration_cast<milliseconds>(end - start).count();
		
		//List find
		start = steady_clock::now();
		for (auto entry : set) {
			if (!list.exists(entry))
				throw std::runtime_error("Problem searching in List ");
		}
		end = steady_clock::now();
		data.search[SLIST_IND] += duration_cast<milliseconds>(end - start).count();

		//tree find
		start = steady_clock::now();
		for (auto entry : set) {
			if (!tree.exists(entry))
				throw std::runtime_error("Problem searching in tree ");
		}
		end = steady_clock::now();
		data.search[AVL_IND] += duration_cast<milliseconds>(end - start).count();

		//List delete
		start = steady_clock::now();
		for (auto entry : set) {
			if (!list.remove(entry))
				throw std::runtime_error("Problem deleting in List ");
		}
		end = steady_clock::now();
		data.deletion[SLIST_IND] += duration_cast<milliseconds>(end - start).count();

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
		data.deletion[AVL_IND] += duration_cast<milliseconds>(end - start).count();
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


int main() {
	srand(time(NULL));
	try {
		TestHelper data = findAvgInsertDelFind(1000);
		std::cout << "Start.\n";
		std::cout << "insertion in AVL:" << data.insertion[AVL_IND] << "ms\n";
		std::cout << "insertion in Skiplist:" << data.insertion[SLIST_IND] << "ms\n";
		std::cout << "Search in AVL:" << data.search[AVL_IND] << "ms\n";
		std::cout << "Search in Skiplist:" << data.search[SLIST_IND] << "ms\n";
		std::cout << "Deletion in AVL:" << data.deletion[AVL_IND] << "ms\n";
		std::cout << "Deletion in Skiplist:" << data.deletion[SLIST_IND] << "ms\n";
		//std::cout << "Getopt for 3000" << getOptimalLvlNum(3000);
		//std::cout << "\ninsertion of 1000 SKIplist:" << testSListInsertion(1000) << "ms";
		std::cout << "\nEnd\n";
	}
	catch (const std::exception& e) {
		std::cout << "\n" << e.what() << "\n";
	}
	AVLTree tree;
	for (int i = 0; i < 10; i++) {
		tree.insert(i);
	}
	std::cout << tree.getSize()<<std::endl;
	for (int i = 0; i < 10; i++) {
		tree.remove(i);
	}
	std::cout << tree.getSize() << std::endl;

}