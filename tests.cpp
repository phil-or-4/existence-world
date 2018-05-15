/*
Author: Phillip Sopt
Description:
This is the set of test cases I made for the GridWorld.h. It should cover all the cases for every function.
Note that these set of test cases were written using Visual Studios testing framework. So if you want to use them,
either convert them to regular old assert commands in C++ or import it into Visual Studio.
*/
#include "stdafx.h"
#include "CppUnitTest.h"
#include "GridWorld.h"
#include <string>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		bool checkPtrs(PersonNode* node1, PersonNode* node2)
		{
			return node1 == node2 ? true : false;
		}
		bool nul(PersonNode* node)
		{
			return (node == nullptr) ? true : false;
		}
		bool nulList(List* list)
		{
			return (list->front == nullptr && list->back == nullptr && list->listLength == 0) ? true : false;
		}
		
		TEST_METHOD(destructor_test)
		{
			GridWorld* gW = new GridWorld(100, 100);
			for (int i = 0; i < 99; i++)
			{
				for (int j = 0; j < rand() % 1000; j++)
				{
					gW->push_back(&(gW->gridArr[rand() % 99][rand() % 99]), 0);

				}
				delete gW;
				gW = new GridWorld(100, 100);
			}
		}

		TEST_METHOD(list_push_back)
		{
			GridWorld gW = GridWorld(1, 1);
			List list;
			gW.push_back(&list, 0);
			Assert::AreEqual(1, list.listLength, L"list length of 1 elem");
			gW.push_back(&list, 1);
			gW.push_back(&list, 2);
			Assert::AreEqual(3, list.listLength, L"list length of 3 elem");
			Assert::AreEqual(0, list.front->id, L"data of first element");
			Assert::AreEqual(true, checkPtrs(list.front, list.front->next->prev), L"check the prev pointer");
			Assert::AreEqual(true, nul(list.front->prev), L"check the prev pointer");
			Assert::AreEqual(true, checkPtrs(list.back, list.back->prev->next), L"[...1 2] 1 -> 2");
			Assert::AreEqual(true, checkPtrs(list.back->prev, list.front->next), L"[...1 2] 1 <- 2");
			Assert::AreEqual(true, nul(list.back->next), L"check the prev pointer");
		}

		TEST_METHOD(list_push_back_node)
		{
			GridWorld gW = GridWorld(1, 1);
			List list;
			PersonNode* node;
			PersonNode temp = PersonNode(-78);
			node = &temp;

			gW.push_back_node(&list, &node);
			Assert::AreEqual(-78, list.front->id, L"-78 id check");
			Assert::AreEqual(1, list.listLength, L"length of one");
			node = new PersonNode(90);
			gW.push_back_node(&list, &node);
			Assert::AreEqual(-78, list.front->id, L"-78 id check");
			Assert::AreEqual(2, list.listLength, L"length of 2");
			Assert::AreEqual(90, list.back->id, L"back of the list");
		}

		TEST_METHOD(list_yank)
		{
			GridWorld gW = GridWorld(1, 1);
			List* list = &gW.gridArr[0][0];
			PersonNode* node;
			int testID = 0;

			//empty yank
			Assert::AreEqual(false, gW.yank_node(list, {}), L"empty yank",LINE_INFO());
			Assert::AreEqual(false, gW.yank_node(list, nullptr), L"empty yank",LINE_INFO());

			
			//one elem yank
			gW.birth(0, 0, testID);		//id = 0
			node = gW.personArr[0].ptrToDLL;
			Assert::AreEqual(1, list->listLength, L"check length");
			Assert::AreEqual(true, gW.yank_node(list, node), L"one elem yank");
			Assert::AreEqual(true, nulList(list), L"check for a null list back");

			Assert::AreEqual(false, gW.yank_node(list, {}), L"empty yank");
			Assert::AreEqual(false, gW.yank_node(list, nullptr), L"empty yank");

			for (int i = 0; i < 8; i++)
			{
				gW.birth(0, 0, testID);
			}
			Assert::AreEqual(8, list->listLength, L"check for 8 length");

			//8 elem yank from front
			node = list->front;
			Assert::AreEqual(true, gW.yank_node(list, node), L"one elem yank");
			Assert::AreEqual(7, list->listLength, L"7 length check");
			Assert::AreEqual(false, checkPtrs(node->next, list->front), L"link check");
			Assert::AreEqual(true, nul(list->front->prev), L"prev link check");

			//back yank
			node = list->back;
			Assert::AreEqual(true, gW.yank_node(list, node), L" yank back");
			Assert::AreEqual(true, nul(list->back->next), L"make sure back.next is nul");
			Assert::AreEqual(6, list->listLength, L"list length is 6");
			Assert::AreEqual(true, nul(node->next), L"nul check on yanked node");
			Assert::AreEqual(true, nul(node->prev), L"nul check on yanked node");

			//middle yank
			node = list->front->next->next->next;	//fourth item in the list
			PersonNode* before = node->prev;
			PersonNode* after = node->next;

			Assert::AreEqual(true, gW.yank_node(list, node), L"yank node middle");
			Assert::AreEqual(true, nul(node->next), L"nul check on yanked node");
			Assert::AreEqual(true, nul(node->prev), L"nul check on yanked node");
			Assert::AreEqual(true, checkPtrs(before->next, after), L"before points to after");
			Assert::AreEqual(true, checkPtrs(after->prev, before), L"after points to before");
			Assert::AreEqual(true, nul(list->front->prev), L"nul check on list front");
			Assert::AreEqual(true, nul(list->back->next), L"nul check on list back");

		}

		TEST_METHOD(test_birth)
		{
			GridWorld gW = GridWorld(1, 1);
			List* list;
			list = &gW.gridArr[0][0];
			PersonNode temp = PersonNode(0);
			PersonNode* node;
			int testID = 0;


			//empty test
			Assert::AreEqual(false, gW.birth(99, 99, testID));
			Assert::AreEqual(false, gW.birth(0, 99, testID));
			Assert::AreEqual(false, gW.birth(0, -99, testID));
			Assert::AreEqual(false, gW.birth(99, 0, testID));
			Assert::AreEqual(false, gW.birth(-99, 0, testID));
			Assert::AreEqual(false, gW.birth(-99, -99, testID));
			Assert::AreEqual(false, gW.birth(1, 0, testID));
			Assert::AreEqual(false, gW.birth(0, 1, testID));
			Assert::AreEqual(false, gW.birth(1, 1, testID));
			Assert::AreEqual(false, gW.birth(-1, 0, testID));
			Assert::AreEqual(false, gW.birth(0, -1, testID));
			Assert::AreEqual(false, gW.birth(-1, -1, testID));

			//first birth
			Assert::AreEqual(true, gW.birth(0, 0, testID), L"birth1");
			Assert::AreEqual(testID, list->front->id, L"check if the id is correct");
			Assert::AreEqual(true, checkPtrs(gW.personArr[0].ptrToDLL, list->front), L"check if the ptr is correct");
			Assert::AreEqual(true, gW.personArr[0].isAlive, L"is Alive check");
			//first death
			gW.death(0);
			//birth him again
			Assert::AreEqual(true, gW.birth(0, 0, testID), L"birth the guy you killed");
			Assert::AreEqual(true, checkPtrs(gW.personArr[0].ptrToDLL,list->front), L"check if the pointer is ok");
			gW.death(0);

			//second birth
			gW.birth(0, 0, testID);	//id 0
			gW.birth(0, 0, testID);	//id 1
			Assert::AreEqual(testID, list->front->next->id, L"second birth id check");
			Assert::AreEqual(true, gW.personArr[0].isAlive, L"is Alive check");
			Assert::AreEqual(true, gW.death(0), L"death0");
			Assert::AreEqual(true, gW.death(1), L"death1");
			Assert::AreEqual(true, gW.birth(0, 0, testID), L"after death birth");	//id is 0
			Assert::AreEqual(0, testID, L"id check for 0 first one");
			gW.death(0);
			gW.birth(0, 0, testID);
			Assert::AreEqual(1, testID, L"id check for 0");


			

		}

		TEST_METHOD(test_death)
		{
			GridWorld gW = GridWorld(1, 1);
			List* list;
			list = &gW.gridArr[0][0];
			PersonNode* node;
			int testID = 0;

			Assert::AreEqual(false, gW.death(0), L"empty death");

			//single  birth then death
			gW.birth(0, 0, testID); 
			Assert::AreEqual(1, gW.numPopulation);
			Assert::AreEqual(true, gW.death(0), L"single death");
			Assert::AreEqual(true, nulList(list), L"null check on grid array");
			Assert::AreEqual(1, gW.deadPool.listLength, L"deadpool of one");
			Assert::AreEqual(false, gW.personArr[0].isAlive, L"person arr check if dead");

			gW.birth(0, 0, testID); 
			Assert::AreEqual(0, testID, L" used dead id 0 ");
			gW.birth(0, 0, testID); 
			Assert::AreEqual(1, testID, L" incrementing like normal, used id 1");
			gW.birth(0, 0, testID); 
			Assert::AreEqual(true, gW.death(0), L"single death");
			Assert::AreEqual(false, gW.death(0), L"kill someone whos dead");
			Assert::AreEqual(true, gW.death(1), L"single death");
			Assert::AreEqual(1, list->listLength, L"1 length test");
			Assert::AreEqual(2, gW.deadPool.listLength, L"deadpool of 2");
			Assert::AreEqual(false, gW.personArr[0].isAlive, L"person 0 arr check if dead");
			Assert::AreEqual(false, nulList(list), L"null check on grid array");

		}

		TEST_METHOD(test_whereis)
		{
			GridWorld gW = GridWorld(10, 10);
			List* list;
			list = &gW.gridArr[0][0];
			PersonNode* node;
			int trow = 0;
			int tcol = 0;

			Assert::AreEqual(false, gW.whereis(0, trow, tcol), L"empty whereis");
			Assert::AreEqual(false, gW.whereis(1, trow, tcol), L"empty whereis");
			Assert::AreEqual(false, gW.whereis(-1, trow, tcol), L"empty whereis");
			Assert::AreEqual(false, gW.whereis(10, trow, tcol), L"empty whereis");

			for (int i = 0; i < 10; i++)
			{
				gW.birth(rand() % 10, rand() % 10, trow);
			}

			for (int i = 0; i < 10; i++)
			{
				Assert::AreEqual(true, gW.whereis(i, trow, tcol));
			}

			Assert::AreEqual(false, gW.whereis(10, trow, tcol));
			gW.birth(3, 9, trow);
			gW.whereis(10, trow, tcol);
			Assert::AreEqual(3, trow, L"row check");
			Assert::AreEqual(9, tcol, L"col check");
		}

		TEST_METHOD(test_move_node)
		{

			GridWorld gW = GridWorld(2, 2);
			List* list1 = &gW.gridArr[0][0];
			List* list2 = &gW.gridArr[1][0];
			List* list3 = &gW.gridArr[0][1];
			List* list4 = &gW.gridArr[1][1];
			int testID = 0;

			PersonNode* node;

			node = list1->front;

			Assert::AreEqual(false, gW.move_node(list1, node, list2), L"empty move");
			gW.birth(0, 0, testID);		//id of 0
			node = gW.personArr[0].ptrToDLL;
			Assert::AreEqual(true, gW.move_node(list1, node, list2), L"move [0] from list1 to list2");
			Assert::AreEqual(true, checkPtrs(node, list2->front), L"make sure list2 has the node");
			Assert::AreEqual(true, nulList(list1), L"make sure list1 is empty");

			
			gW.birth(0, 0, testID);//id of 1
			gW.birth(0, 0, testID);//id of 2
			gW.birth(0, 0, testID);//id of 3
			PersonNode* node2 = gW.personArr[2].ptrToDLL;	//node2 is id 2

			Assert::AreEqual(true, gW.move_node(list1, node2, list2), L"middle move");
			Assert::AreEqual(true, checkPtrs(node2, list2->back), L"make sure list2 has the node");
			Assert::AreEqual(true, nul(node2->next), L"make sure list2 has the node");
			Assert::AreEqual(true, checkPtrs(node2->prev, list2->front), L"make sure list2 has the node");
		}

		TEST_METHOD(test_move)
		{
			GridWorld gW = GridWorld(2, 2);
			List* list = &gW.gridArr[0][0];
			int testID = 0;

			Assert::AreEqual(false, gW.move(0, 0, 0), L"empty mov");
			Assert::AreEqual(true, nulList(list), L" make sure the list is empty");

			gW.birth(0, 0, testID);	//id of 0
			Assert::AreEqual(true, gW.move(0, 0, 0), L"same mov");
			Assert::AreEqual(false, nulList(list));
			gW.birth(0, 0, testID); //id of 1
			gW.move(0, 0, 0);	//inplace move
			Assert::AreEqual(true, checkPtrs(list->front, gW.personArr[1].ptrToDLL), L"make sure [1 0]");
			Assert::AreEqual(true, checkPtrs(list->front->next, gW.personArr[0].ptrToDLL), L"make sure [1 0]");

			gW.birth(0, 1, testID); //id of 2
			Assert::AreEqual(true, gW.move(testID, 1, 1), L"mov testId to a different location");
			Assert::AreEqual(testID, gW.gridArr[1][1].front->id, L"check if the id is moved");
			Assert::AreEqual(1, gW.personArr[testID].row, L"row check");
			Assert::AreEqual(1, gW.personArr[testID].col, L"col check");
			Assert::AreEqual(true, gW.personArr[testID].isAlive, L"alive check");

			gW.birth(0, 1, testID); //id of 3
			Assert::AreEqual(true, gW.move(testID, 1, 1), L"mov testId to a different location");
			Assert::AreEqual(true, checkPtrs(gW.personArr[2].ptrToDLL->next, gW.personArr[3].ptrToDLL), L"[2 3] 2->3 after moving 2");
			Assert::AreEqual(true, checkPtrs(gW.personArr[3].ptrToDLL->prev, gW.personArr[2].ptrToDLL), L"[2 3] 2<-3 after moving 2");

			



		}
	};
}