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


int main() {
	srand(time(NULL));
	try {
		std::cout << "Start.\n";
		std::cout << "insertion of 1000 AVL:" << testAVLInsertion(1000) << "ms";
		//std::cout << "Getopt for 1000" << getOptimalLvlNum(1000);
		std::cout << "\ninsertion of 1000 SKIplist:" << testSListInsertion(1000, 10) << "ms";
		std::cout << "\nEnd\n";
	}
	catch (const std::exception& e) {
		std::cout << "\n" << e.what() << "\n";
	}
}