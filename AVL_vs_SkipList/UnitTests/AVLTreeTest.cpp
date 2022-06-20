#define CATCH_CONFIG_MAIN  
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <unordered_set>
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
			int values[1000];
			for (int i = 0; i < 1000; i++) {
				bool isSet = true;
				do {
					values[i] = rand() % 100000;
					if (i % 2) values[i] *= -1;
				} while (set.find(values[i])!=set.end());//create unique elements

				set.insert(values[i]);
				REQUIRE(tree.insert(values[i]));
			}

			THEN("Test finding the elements") {
				for (int i = 0; i < 1000; i++)
					REQUIRE(tree.exists(values[i]));
			}
			THEN("Test for getSize") {
				REQUIRE(tree.getSize() == 1000);
			}//then
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
			int values[1000];
			std::unordered_set<int> set;
			for (int i = 0; i < 1000; i++) {
				do {
					values[i] = rand() % 100000;
					if (i % 2) values[i] *= -1;
				} while (set.find(values[i]) != set.end());//create unique elements
				set.insert(values[i]);
				tree.insert(values[i]);
			}
			THEN("Test removing half of them") {
				REQUIRE(tree.getSize() == 1000);
				for (int i = 0; i < 500; i++) {
					tree.remove(values[i]);
				}
				for (int i = 0; i < 500; i++) {
					REQUIRE(!tree.exists(values[i]));

				}
				for (int i = 500; i < 1000; i++) {
					REQUIRE(tree.exists(values[i]));

				}
				REQUIRE(tree.getSize() == 500);
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
			int values[1000];
			for (int i = 0; i < 1000; i++) {
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
					for (int i = 0; i < 1000; i++)
						REQUIRE(copyTree.exists(values[i]));
				}
			}//then
			THEN("Make a copy with oper=") {
				AVLTree copyTree;
				copyTree = tree;
				AND_THEN("Test if copy has the correct values") {
					for (int i = 0; i < 1000; i++)
						REQUIRE(copyTree.exists(values[i]));
				}
			}//then
		}//when
	}//given
	GIVEN("Test move constr and move operator=") {
		AVLTree tree;
		WHEN("Fill the default tree with 1000 random values") {
			std::unordered_set<int> set;
			int values[1000];
			for (int i = 0; i < 1000; i++) {
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
		WHEN("Insert elements") {
			for (int i = 0; i < 1000; i++) {
				tree.insert(i);
			}
			REQUIRE(tree.getSize() == 1000);
			int cnt = 0;
			THEN("Check if iteration is correct with begin and end") {
				for (auto node : tree) {
					REQUIRE(node->value == cnt++);
				}
			}
		}
	}//given
}//scen

