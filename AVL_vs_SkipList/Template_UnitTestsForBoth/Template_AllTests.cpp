#define CATCH_CONFIG_MAIN  
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <unordered_set>
#include <cmath>
//
#include "catch.hpp"
#include "../Template_AVL_SkipList/T_AVLTree.h" 
#include "../Template_AVL_SkipList/T_SkipList.h" 



SCENARIO("Testing AVLTree<int> class insertion") {
	srand(time(NULL));
	GIVEN("Creating default object") {
		AVLTree<int> tree;
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

SCENARIO("Testing AVLTree<int> class deletion") {
	srand(time(NULL));
	GIVEN("Creating default object") {
		AVLTree<int> tree;
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
			THEN("Test removing all of them") {
				REQUIRE(tree.getSize() == TEST_NUM);
				for (int i = 0; i < TEST_NUM; i++) {
					REQUIRE(tree.remove(values[i]));
				}
				for (int i = 0; i < TEST_NUM; i++) {
					REQUIRE(!tree.exists(values[i]));

				}
				REQUIRE(tree.getSize() == 0);
			}

		}//when
	}//given
}//scen

SCENARIO("Test AVLTree<int> class operators and constructors")
{
	GIVEN("Test copy constr and copy operator=") {
		AVLTree<int> tree;
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
				AVLTree<int> copyTree = tree;
				AND_THEN("Test if copy has the correct values") {
					for (int i = 0; i < TEST_NUM; i++)
						REQUIRE(copyTree.exists(values[i]));
				}
			}//then
			THEN("Make a copy with oper=") {
				AVLTree<int> copyTree;
				copyTree = tree;
				AND_THEN("Test if copy has the correct values") {
					for (int i = 0; i < TEST_NUM; i++)
						REQUIRE(copyTree.exists(values[i]));
				}
			}//then
		}//when
	}//given
	GIVEN("Test move constr and move operator=") {
		AVLTree<int> tree;
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
				AVLTree<int> movedtree = std::move(tree);
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
				AVLTree<int> movedtree;
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
SCENARIO("Testing AVLTree<int> class iteration left-parent-right") {
	GIVEN("Create default tree") {
		AVLTree<int> tree;
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

//list

SCENARIO("Testing SkipList<int> class insertion") {
	srand(time(NULL));
	GIVEN("Creating default object with 4 lvls and 0.5 fraction") {
		SkipList<int> slist(4, 0.5);
		WHEN("Insert 3 elements") {
			REQUIRE(slist.insert(1));
			REQUIRE(slist.insert(3));
			REQUIRE(slist.insert(2));
			THEN("Test finding the elements") {
				REQUIRE(slist.exists(1));
				REQUIRE(slist.exists(2));
				REQUIRE(slist.exists(3));
				REQUIRE(slist.exists(4) == false);
			}
			THEN("Test for getSize") {
				REQUIRE(slist.getSize() == 3);
			}//then
		}//when
		WHEN("Test inserting dublicates") {
			REQUIRE(slist.insert(3));
			REQUIRE(!slist.insert(3));
			REQUIRE(!slist.insert(3));
		}
		WHEN("Insert 1000 random different elements") {
			std::unordered_set<int> set;
			const int TEST_NUM = 1000;
			int values[TEST_NUM];
			for (int i = 0; i < TEST_NUM; i++) {
				bool isSet = true;
				do {
					values[i] = rand() % 100000;
					if (i % 2) values[i] *= -1;
				} while (set.find(values[i]) != set.end());//create unique elements
				//std::cout << i<<" ";
				set.insert(values[i]);
				REQUIRE(slist.insert(values[i]));
			}
			//std::cout << "height:" << slist.getHeight();

			THEN("Test finding the elements") {
				for (int i = 0; i < TEST_NUM; i++)
					REQUIRE(slist.exists(values[i]));
			}
			THEN("Test for getSize") {
				REQUIRE(slist.getSize() == TEST_NUM);
			}//then
			THEN("Test for clear functon") {
				slist.clearData();
				REQUIRE(slist.getSize() == 0);
				for (int i = 0; i < TEST_NUM; i++)
					REQUIRE(!slist.exists(values[i]));
				AND_THEN("Test for reinsert") {
					for (int i = 0; i < TEST_NUM / 4; i++) {
						REQUIRE(slist.insert(values[i]));
					}
					for (int i = 0; i < TEST_NUM / 4; i++) {
						REQUIRE(slist.exists(values[i]));
					}
				}
			}//then

		}//when
	}//given
}//scen

SCENARIO("Testing SkipList<int> class deletion") {
	srand(time(NULL));
	GIVEN("Creating default object on 4 lvls and 0.5 fraction") {
		SkipList<int> slist(4, 0.5);
		WHEN("Insert 3 elements") {
			slist.insert(1);
			slist.insert(3);
			slist.insert(2);
			THEN("Try removing them") {
				REQUIRE(slist.exists(2));
				slist.remove(2);
				REQUIRE(!slist.exists(2));
				REQUIRE(slist.getSize() == 2);
				REQUIRE(slist.exists(1));
				slist.remove(1);
				REQUIRE(!slist.exists(1));
				REQUIRE(slist.getSize() == 1);
				REQUIRE(slist.exists(3));
				slist.remove(3);
				REQUIRE(!slist.exists(3));
				REQUIRE(slist.getSize() == 0);
			}
		}//when
		WHEN("Try insert, delete and reinsert same element") {
			REQUIRE(slist.insert(3));
			REQUIRE(slist.exists(3));
			REQUIRE(slist.remove(3));
			REQUIRE(slist.exists(3) == false);
			REQUIRE(slist.insert(3));
			REQUIRE(slist.exists(3));
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
				slist.insert(values[i]);
			}
			THEN("Test removing half of them") {
				REQUIRE(slist.getSize() == TEST_NUM);
				for (int i = 0; i < TEST_NUM / 2; i++) {
					slist.remove(values[i]);
				}
				for (int i = 0; i < TEST_NUM / 2; i++) {
					REQUIRE(!slist.exists(values[i]));

				}
				for (int i = TEST_NUM / 2; i < TEST_NUM; i++) {
					//std::cout << i << " ";;
					REQUIRE(slist.exists(values[i]));

				}
				REQUIRE(slist.getSize() == TEST_NUM / 2);
			}

		}//when
	}//given
}//scen

SCENARIO("Test SkipList<int> class operators and constructors")
{
	GIVEN("Test copy constr and copy operator=") {
		SkipList<int> slist(4, 0.5);
		WHEN("Fill the default slist with 1000 random values") {
			std::unordered_set<int> set;
			const int TEST_NUM = 1000;
			int values[TEST_NUM];
			for (int i = 0; i < TEST_NUM; i++) {
				do {
					values[i] = rand() % 100000;
					if (i % 2) values[i] *= -1;
				} while (set.find(values[i]) != set.end());//create unique elements
				slist.insert(values[i]);
				set.insert(values[i]);
			}
			THEN("Make a copy with constr") {
				SkipList<int> copyslist = slist;
				AND_THEN("Test if copy has the correct values") {
					for (int i = 0; i < TEST_NUM; i++)
						REQUIRE(copyslist.exists(values[i]));
				}
			}//then
			THEN("Make a copy with oper=") {
				SkipList<int> copyslist(4, 0.5);
				copyslist = slist;
				AND_THEN("Test if copy has the correct values") {
					for (int i = 0; i < TEST_NUM; i++)
						REQUIRE(copyslist.exists(values[i]));
				}
			}//then
		}//when
	}//given
	GIVEN("Test move constr and move operator=") {
		SkipList<int> slist(4, 0.5);
		WHEN("Fill the default slist with 1000 random values") {
			std::unordered_set<int> set;
			const int TEST_NUM = 1000;
			int values[TEST_NUM];
			for (int i = 0; i < TEST_NUM; i++) {
				do {
					values[i] = rand() % 100000;
					if (i % 2) values[i] *= -1;
				} while (set.find(values[i]) != set.end());//create unique elements
				slist.insert(values[i]);
				set.insert(values[i]);
			}
			THEN("Move with constr") {
				SkipList<int> movedslist = std::move(slist);
				AND_THEN("Test if old slist is empty") {
					for (int i = 0; i < TEST_NUM; i++)
						REQUIRE(!slist.exists(values[i]));
					REQUIRE(slist.getSize() == 0);
				}
				AND_THEN("Test if new list is moved") {
					for (int i = 0; i < TEST_NUM; i++)
						REQUIRE(movedslist.exists(values[i]));
					REQUIRE(movedslist.getSize() == TEST_NUM);
				}
			}//then
			THEN("Move with oper=") {
				SkipList<int> movedslist(4, 0.5);
				movedslist = std::move(slist);
				AND_THEN("Test if old list is empty") {
					for (int i = 0; i < 1000; i++)
						REQUIRE(!slist.exists(values[i]));
					REQUIRE(slist.getSize() == 0);
				}
				AND_THEN("Test if new slist is moved") {
					for (int i = 0; i < 1000; i++)
						REQUIRE(movedslist.exists(values[i]));
					REQUIRE(movedslist.getSize() == 1000);
				}
			}//then
		}//when
	}//given

}//scen
SCENARIO("Testing SkipList<int> class iteration left-parent-right") {
	GIVEN("Create default slist") {
		SkipList<int> slist(4, 0.5);
		const int TEST_NUM = 1000;
		WHEN("Insert elements") {
			for (int i = TEST_NUM - 1; i >= 0; i--) {
				slist.insert(i);
			}
			REQUIRE(slist.getSize() == TEST_NUM);
			int cnt = 0;
			THEN("Check if iteration is correct with begin and end") {
				for (auto node : slist) {
					REQUIRE(node->value == cnt);
					cnt++;
				}
			}
		}
	}//given
}//scen

