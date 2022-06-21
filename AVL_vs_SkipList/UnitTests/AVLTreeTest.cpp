#define CATCH_CONFIG_MAIN  
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <unordered_set>
#include <cmath>
//
#include "catch.hpp"
#include "../AVL_vs_SkipList/AVLTree.h"


SCENARIO("Testing AVLTree class insertion") {
	srand(time(NULL));
	GIVEN("Creating default object") {
		AVLTree tree;
		WHEN("Insert 3 elements") {
			REQUIRE(tree.insert(1));
			REQUIRE(tree.insert(3));
			REQUIRE(tree.insert(2));
			THEN("Test finding the elements") {
				REQUIRE(tree.exists(1));
				REQUIRE(tree.exists(2));
				REQUIRE(tree.exists(3));
				REQUIRE(tree.exists(4) == false);
			}
			THEN("Test for getSize") {
				REQUIRE(tree.getSize() == 3);
			}//then
		}//when
		WHEN("Test inserting dublicates") {
			REQUIRE(tree.insert(3));
			REQUIRE(!tree.insert(3));
			REQUIRE(!tree.insert(3));
		}
		WHEN("Insert 1000 random elements") {
			std::unordered_set<int> set;
			const int TEST_NUM = 1000;
			int values[TEST_NUM];
			for (int i = 0; i < TEST_NUM; i++) {
				bool isSet = true;
				do {
					values[i] = rand() % 100000;
					if (i % 2) values[i] *= -1;
				} while (set.find(values[i])!=set.end());//create unique elements

				set.insert(values[i]);
				REQUIRE(tree.insert(values[i]));
			}
			//std::cout << "height:" << tree.getHeight();

			THEN("Test finding the elements") {
				for (int i = 0; i < TEST_NUM; i++)
					REQUIRE(tree.exists(values[i]));
			}
			THEN("Test for getSize") {
				REQUIRE(tree.getSize() == TEST_NUM);
			}//then
			THEN("Test for correct height") {
				REQUIRE(tree.getHeight() <= 1.5*log2(TEST_NUM));
			}
		}//when
	}//given
}//scen

SCENARIO("Testing AVLTree class deletion") {
	srand(time(NULL));
	GIVEN("Creating default object") {
		AVLTree tree;
		WHEN("Insert 3 elements") {
			tree.insert(1);
			tree.insert(3);
			tree.insert(2);
			THEN("Try removing them") {
				REQUIRE(tree.exists(2));
				tree.remove(2);
				REQUIRE(!tree.exists(2));
				REQUIRE(tree.getSize() == 2);
				REQUIRE(tree.exists(1));
				tree.remove(1);
				REQUIRE(!tree.exists(1));
				REQUIRE(tree.getSize() == 1);
				REQUIRE(tree.exists(3));
				tree.remove(3);
				REQUIRE(!tree.exists(3));
				REQUIRE(tree.getSize() == 0);
			}
		}//when
		WHEN("Try insert, delete and reinsert same element") {
			REQUIRE(tree.insert(3));
			REQUIRE(tree.exists(3));
			REQUIRE(tree.remove(3));
			REQUIRE(tree.exists(3) == false);
			REQUIRE(tree.insert(3));
			REQUIRE(tree.exists(3));
		}
		WHEN("Insert 1000 random elements") {
			const int TEST_NUM = 1000;
			int values[TEST_NUM];
			std::unordered_set<int> set;
			for (int i = 0; i < TEST_NUM; i++) {
				do {
					values[i] = rand() % 100000;
					if (i % 2) values[i] *= -1;
				} while (set.find(values[i]) != set.end());//create unique elements
				set.insert(values[i]);
				tree.insert(values[i]);
			}
			THEN("Test removing half of them") {
				REQUIRE(tree.getSize() == TEST_NUM);
				for (int i = 0; i < TEST_NUM/2; i++) {
					tree.remove(values[i]);
				}
				for (int i = 0; i < TEST_NUM/2; i++) {
					REQUIRE(!tree.exists(values[i]));

				}
				for (int i = TEST_NUM/2; i < TEST_NUM; i++) {
					//std::cout << i << " ";;
					REQUIRE(tree.exists(values[i]));

				}
				REQUIRE(tree.getSize() == TEST_NUM/2);
			}

		}//when
	}//given
}//scen

SCENARIO("Test AVLTree class operators and constructors")
{
	GIVEN("Test copy constr and copy operator=") {
		AVLTree tree;
		WHEN("Fill the default tree with 1000 random values") {
			std::unordered_set<int> set;
			const int TEST_NUM = 1000;
			int values[TEST_NUM];
			for (int i = 0; i < TEST_NUM; i++) {
				do {
					values[i] = rand() % 100000;
					if (i % 2) values[i] *= -1;
				} while (set.find(values[i]) != set.end());//create unique elements
				tree.insert(values[i]);
				set.insert(values[i]);
			}
			THEN("Make a copy with constr") {
				AVLTree copyTree = tree;
				AND_THEN("Test if copy has the correct values") {
					for (int i = 0; i < TEST_NUM; i++)
						REQUIRE(copyTree.exists(values[i]));
				}
			}//then
			THEN("Make a copy with oper=") {
				AVLTree copyTree;
				copyTree = tree;
				AND_THEN("Test if copy has the correct values") {
					for (int i = 0; i < TEST_NUM; i++)
						REQUIRE(copyTree.exists(values[i]));
				}
			}//then
		}//when
	}//given
	GIVEN("Test move constr and move operator=") {
		AVLTree tree;
		WHEN("Fill the default tree with 1000 random values") {
			std::unordered_set<int> set;
			const int TEST_NUM = 1000;
			int values[TEST_NUM];
			for (int i = 0; i < TEST_NUM; i++) {
				do {
					values[i] = rand() % 100000;
					if (i % 2) values[i] *= -1;
				} while (set.find(values[i]) != set.end());//create unique elements
				tree.insert(values[i]);
				set.insert(values[i]);
			}
			THEN("Move with constr") {
				AVLTree movedtree = std::move(tree);
				AND_THEN("Test if old tree is empty") {
					for (int i = 0; i < TEST_NUM; i++)
						REQUIRE(!tree.exists(values[i]));
					REQUIRE(tree.getSize() == 0);
				}
				AND_THEN("Test if new tree is moved") {
					for (int i = 0; i < TEST_NUM; i++)
						REQUIRE(movedtree.exists(values[i]));
					REQUIRE(movedtree.getSize() == TEST_NUM);
				}
			}//then
			THEN("Move with oper=") {
				AVLTree movedtree;
				movedtree = std::move(tree);
				AND_THEN("Test if old tree is empty") {
					for (int i = 0; i < 1000; i++)
						REQUIRE(!tree.exists(values[i]));
					REQUIRE(tree.getSize() == 0);
				}
				AND_THEN("Test if new tree is moved") {
					for (int i = 0; i < 1000; i++)
						REQUIRE(movedtree.exists(values[i]));
					REQUIRE(movedtree.getSize() == 1000);
				}
			}//then
		}//when
	}//given

}//scen
SCENARIO("Testing AVLTree class iteration left-parent-right") {
	GIVEN("Create default tree") {
		AVLTree tree;
		const int TEST_NUM = 1000;
		bool test_arr[TEST_NUM] = {};
		WHEN("Insert elements") {
			for (int i = 0; i < TEST_NUM; i++) {
				tree.insert(i);
			}
			REQUIRE(tree.getSize() == TEST_NUM);
			int cnt = 0;
			THEN("Check if iteration is correct with begin and end") {
				for (auto node : tree) {
					cnt++;
					test_arr[node->value] = true;
				}
			}
			REQUIRE(cnt == TEST_NUM);
			for (int i = 0; i < TEST_NUM; i++) {
				REQUIRE(test_arr[i]);
			}
		}
	}//given
}//scen

