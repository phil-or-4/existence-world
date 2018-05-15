/*
Author: Phillip Sopt

Description: This program is an excercise in data structure knowledge. Our goal was to
create a program that would populate a 2D world that is divided into districts like
an x,y coordinate grid. Each district was to have a population and each person a
unique id, like a social security number. If no dead ids are avaliable then a new born
person would get a new id. If a person was to die then his id was to be reused. The 
oldest dead id was to be used by the next new born person. The runtime for each function
was to be constant time, with exception to members.


*/
#include "GWInterface.h"
#include <vector>
#include <iostream>

using std::vector;

//used for the nodes in the doubly linked list, and for deadPool
class PersonNode {
public:
	int id;
	PersonNode* next = nullptr;
	PersonNode* prev = nullptr;
	PersonNode(int id, PersonNode * n = nullptr, PersonNode* p = nullptr)
	{
		this->id = id;
		next = n;
		prev = p;
	}
};

//use for the person array
class PersonArrayObj {
public:
	int id;
	int row;
	int col;
	PersonNode* ptrToDLL = nullptr;
	bool isAlive = false;
	PersonArrayObj(int id, int row, int col, PersonNode* ptrToDLL, bool isAlive)
	{
		this->id = id;
		this->row = row;
		this->col = col;
		this->ptrToDLL = ptrToDLL;
		this->isAlive = isAlive;
	}
};

//used for doubly linked list
struct List {
	PersonNode* front = nullptr;
	PersonNode* back = nullptr;
	int listLength = 0;
};

class GridWorld : public GWInterface {

  public:
    GridWorld(unsigned nrows, unsigned ncols)   {
		//allocate space for the gridArr of Lists
		gridArr = new List*[nrows];
		for (int i = 0; i < nrows; i++)
		{
			gridArr[i] = new List[ncols];
		}
		numPopulation = 0;
		this->nrows = nrows;
		this->ncols = ncols;
    }
    ~GridWorld(){
		//go through all the districts and free the nodes up
		for (int i = 0; i < nrows; i++)
		{
			for (int j = 0; j < ncols; j++)
			{
				while (gridArr[i][j].front != nullptr)
				{
					PersonNode* temp = gridArr[i][j].front;
					if (yank_node(&gridArr[i][j],temp))
					{
						delete temp;
					}
				}
			}
		}
		//go through the gridArr and delete the memory allocated
		for (int i = 0; i < nrows; i++)
		{
			delete[] gridArr[i];
		}
		delete[] gridArr;
    }


	//runtime of O(1)
	bool birth(int row, int col, int &id){
		
		//if the row and col are outside of the bounds
		if (row < 0 || row >= nrows || col < 0 || col >= ncols)
		{
			return false;
		}

		//if the deadPool is empty
		if (deadPool.listLength == 0)
		{
			//push_back the data with the numPopulation, and place him in {row,col} district
			push_back(&gridArr[row][col], numPopulation);
			id = numPopulation;
			personArr.push_back( PersonArrayObj(id, row, col, gridArr[row][col].back, true) );
			numPopulation += 1;
		}
		//if the deadPool is not empty
		else
		{
			//create a person with the id of the first thing in the dead pool
			PersonNode* temp = deadPool.front;									//save the node to that first deadPool object
			id = temp->id;													//set the id to that dead id
			push_back(&gridArr[row][col], deadPool.front->id);
			personArr[id] = PersonArrayObj(id, row, col, gridArr[row][col].back, true );		//reset the personArrayObj
			if(!yank_node(&deadPool, temp)) return false;						//pop the oldest dead guy off deadpool
			delete temp;														//delete him
		}

		return true;
    }

	//runtime of O(1)
	bool death(int personID){

		if (personID >= personArr.size() || personID < 0 || personArr.size() == 0)	return false;	//bounds check

		PersonArrayObj* temp = &personArr[personID];		//temp pointer to make things eaiser

		//if that persone is alive then end them
		if (temp->isAlive)
		{
			if(!yank_node(&gridArr[temp->row][temp->col], temp->ptrToDLL)) return false;	//yank temp's pointer from the district list
			delete temp->ptrToDLL;				//delete the node
			push_back(&deadPool, temp->id);	//push temp to the deadpool
			temp->isAlive = false;				//set temps isAlive flag
		}
		//if the person we want to end is already dead then return false
		else 
		{
			return false; 
		}

		return true;
    }

	//runtime of O(1)
	bool whereis(int id, int &row, int &col)const{

		if (id >= personArr.size() || id < 0 || personArr.size() == 0)	return false;		//bounds check

		//check if the guy is even alive first
		if (personArr[id].isAlive)
		{
			//simply return his data
			row = personArr[id].row;
			col = personArr[id].col;
			return true;
		}

		return false;
    }

	//runtime of O(1)
	bool move(int id, int targetRow, int targetCol){

		if (id >= personArr.size() || id < 0 || personArr.size() == 0)	return false;		//bounds check
		
		//if the row and col are outside of the bounds
		if (targetRow < 0 || targetRow >= nrows || targetCol < 0 || targetCol >= ncols)
		{
			return false;
		}
		
		//store the Person array object
		PersonArrayObj* temp = &personArr[id];

		if (temp->isAlive == false) return false;

		//if move node from nodes origin list to the target list is successful
		if (move_node(&gridArr[temp->row][temp->col],temp->ptrToDLL,&gridArr[targetRow][targetCol]))
		{
			temp->row = targetRow;
			temp->col = targetCol;
			return true;
		}

		return false;
    }

	//runtime of O(N(r,c)) where N is the population of that district
	std::vector<int> * members(int row, int col)const{

		//store the result
		vector<int> *result = new vector<int>();

		//if the row and col are outside of the bounds
		if (row < 0 || row >= nrows || col < 0 || col >= ncols)
		{
			return result;
		}

		PersonNode* temp = gridArr[row][col].front;

		//loop through the list at that district
		while (temp != nullptr)
		{
			//push back the result
			result->push_back(temp->id);

			//increment the temp pointer
			temp = temp->next;
		}

		return result;
    }

	//runtime of O(1)
    int population()const{
		//simply return the population counter - the people in the deadPool
		return numPopulation - deadPool.listLength;
    }
    
	//runtime of O(1)
    int population(int row, int col)const{

		//if the row and col are outside of the bounds
		if (row < 0 || row >= nrows || col < 0 || col >= ncols)
		{
			return 0;
		}
		return gridArr[row][col].listLength;
    }

	//runtime of O(1)
    int num_rows()const {
      return nrows;
    }
	//runtime of O(1)
    int num_cols()const {
      return ncols;
    }

private:

	//push_back will take a list and data and then push that data in the list and return the node*
	//it returns a node* so the person arr with have something to point to
	PersonNode* push_back(List* list, int id)
	{
		PersonNode *tmp = new PersonNode(id, nullptr, nullptr);
		list->listLength += 1;

		if (list->front == nullptr)
		{
			list->front = list->back = tmp;
		}
		else
		{
			list->back->next = tmp;
			tmp->prev = list->back;
			list->back = tmp;
		}

		return tmp;
	}

	//this push_back is a little different, it takes a node ptr and puts it in a list
	bool push_back_node(List* list, PersonNode** node)
	{
		//if the list is empty
		if (list->front == nullptr)
		{
			//make node the only elem
			list->front = list->back = *node;
			(*node)->next = nullptr;
			(*node)->prev = nullptr;
		}
		else
		{
			//make node the back of the list
			list->back->next = *node;
			(*node)->prev = list->back;
			list->back = *node;
			(*node)->next = nullptr;
		}
		list->listLength += 1;
		return true;
	}

	//yank_node is used to decouple a PersonNode* from a list
	bool yank_node(List* list, PersonNode* node)
	{
		//if the list is empty forget it
		if (list->front == nullptr)
		{
			return false;
		}

		//if node is the only elem of list1
		if (node == list->front && node == list->back)
		{
			//make front and back nullptr
			list->front = list->back = nullptr;
		}
		//if node is at the front of list1
		else if (node == list->front)
		{
			//make front point to node.next
			list->front = node->next;
			list->front->prev = nullptr;
		}
		//if node is at the back of the list
		else if (node == list->back)
		{
			//make the node before node be the new back
			list->back->prev->next = nullptr;
			list->back = node->prev;

		}
		//if anything else
		else
		{
			//make nodes.prev point to node.next & node.next point back to node.prev
			node->prev->next = node->next;
			node->next->prev = node->prev;
		}

		node->next = nullptr;
		node->prev = nullptr;
		list->listLength -= 1;
		return true;
	}

	//move_node takes two lists and yanks the node from list1 and moves it into list2
	bool move_node(List* list1, PersonNode* node, List* list2)
	{
		//check to see if the node is yankable
		if (!yank_node(list1, node)) return false;

		//push the node back in the other list
		if (push_back_node(list2, &node))
		{
			return true;
		}

		return false;
	}

		
	//this is the grid of Doubly Linked List(DLL's) for each distric
	//this is used to get the O(1) runtime for move
	List** gridArr;
	int numPopulation;
	int nrows;
	int ncols;
	vector<PersonArrayObj> personArr;			//this is the person array
	List deadPool;								//this is the used to hold all the dead id's
};
