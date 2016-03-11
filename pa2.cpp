#include <iostream>
#include <cstring>
#include <string>
#include "pa2.h"
using namespace std;

//This function is to print the menu for the user
void printMenu()
{
	std::cout << "\t1. Add program\n\t2. Kill program\n\t3. Fragmentation"
		<< "\n\t4. Print memory\n\t5. Exit\n";
}

/* Main function is supposed to accept two arguments only:
   One for the execution of the program, the other is one 
   is the choice of memory management method. (best or worst)
*/ 
int main(int argc, char *argv[])
{
	int startCode = 0; //to determine the memory management method
	
	if (argc == 2 && argv[1] != 0)
	{
		if (strcmp(argv[1], "best") == 0)
		{
			cout << "Using best fit algorithm\n" << endl;
			startCode = 1;
		}
		else if (strcmp(argv[1], "worst") == 0)
		{
			cout << "Using worst fit algorithm\n" << endl;
			startCode = 2;
		}
		else
		{
			cout << "Your command was either empty or invalid. "
				<< "Using best fit algorithm by default" << endl;
			startCode = 1;
		}
	}
	else
	{
		cout << "Please run this program with only one of these arguments: best or worst" << endl;
		
		return 1; //exit the program with abnormal exit code, 1
	}

	printMenu();
	//Declare and initial the two LinkedLists
	LinkedList* free = new LinkedList("Free");
	LinkedList* used = new LinkedList();

	int choice = -1;

	if (startCode == 1)
	{
		while (choice != 5)
		{
			choice = 0;
			cout << "choice - ";
			cin >> choice;
			processBest(choice, free, used); //call function to process 
		}
	}
	else if (startCode == 2)
	{
		while (choice != 5)
		{
			choice = 0;
			cout << "choice - ";
			cin >> choice;
			processWorst(choice, free, used); //call function to process 
		}
	}

	//system("pause"); Discarded; for debug with Visual Studio
	return 0; //exit the program with normal exit code, 0
}

/* This function is for the sake of reducing the length of Main function
   This function is to process the user's choice from the menu with
   Best fit memory management method
*/
void processBest(int choice, LinkedList *f, LinkedList *u)
{
	string name;
	int size;

	switch (choice)
	{
		case 1:
			cout << "Program name - ";
			cin >> name;
			cout << "Program size (KB) - ";
			cin >> size;
			addBest(f, u, name, size);
			break;
		case 2:
			cout << "\nProgram name - ";
			cin >> name;
			killProgram(f, u, name);
			break;
		case 3:
			fragments(f);
			break;
		case 4:
			printMem(f, u);
			break;
		case 5:
			break;
		default:
			cout << "Please enter the number from the menu only!" << endl;
			cin.clear();
			cin.ignore(10000, '\n');
	}
}

/* This function is for the sake of reducing the length of Main function
   This function is to process the user's choice from the menu with
   Worst fit memory management method
*/
void processWorst(int choice, LinkedList *f, LinkedList *u)
{
	string name;
	int size;
	//cout << "Here in ProcessWorst" << endl;
	switch (choice)
	{
	case 1:
		cout << "\nProgram name - ";
		cin >> name;
		cout << "Program size (KB) - ";
		cin >> size;
		addWorst(f, u, name, size);
		break;
	case 2:
		cout << "\nProgram name - ";
		cin >> name;
		killProgram(f, u, name);
		break;
	case 3:
		fragments(f);
		break;
	case 4:
		//printList(f);
		//printList(u);
		printMem(f, u);
		break;
	case 5:
		break;
	default:
		cout << "Please enter the number from the menu only!" << endl;
		cin.clear();
		cin.ignore(10000, '\n');
	}

	//cout << "Return from ProcessWorst" << endl;
}

/* This function is called to add an item ( ListNode ) to the database
   using the Best fit memory management method
   At the same time update both of the LinkedLists
*/
void addBest(LinkedList *f, LinkedList *u, string name, int count)
{
	double temp = (double)count / 4 + 0.99999;
	int page = (int)temp;
	//cout << "Here in AddBest" << endl;
	ListNode* target = 0;
	ListNode* targetP = 0;
	ListNode* previous = f->getHead();
	ListNode* next = previous->getNext();
	int smallest = 33;
	do
	{
		if (next->getCount() >= page && next->getCount() < smallest && next->getName().compare("Free") == 0)
		{
			smallest = next->getCount();
			target = next;
			targetP = previous;
		}
		previous = next;
		next = next->getNext();
	} while (next != 0);

	if (smallest == 33 || target == 0)
		cout << "Error, Not enough memory for Program " << name << endl;
	else
	{
		ListNode* item;
		ListNode* last;
		ListNode* slot = u->findSlot(f, target);

		if (count > 8)
		{
			item = new ListNode(name, 4);
			count -= 4;
			last = new ListNode(name, 4);
			item->setNext(last);
			count -= 4;
			while (count > 4)
			{
				ListNode* t = new ListNode(name, 4);
				last->setNext(t);
				count -= 4;
				last = t;
			}
			ListNode* t2 = new ListNode(name, count);
			last->setNext(t2);
			last = t2;
		}
		else if (count > 4)
		{
			item = new ListNode(name, 4);
			count -= 4;
			ListNode* t3 = new ListNode(name, count);
			item->setNext(t3);
			last = t3;
		}
		else
		{
			item = new ListNode(name, count);
			last = item;
		}
		last->setNext(slot->getNext());
		slot->setNext(item);


		ListNode* split = new ListNode("Used", page);
		target->setCount(target->getCount() - page);
		split->setNext(target);
		targetP->setNext(split);

	}
	//cout << "Return from AddBest" << endl;
}

/* This function is called to add an item ( ListNode ) to the database
   using the Worst fit memory management method
   At the same time update both of the LinkedLists
*/
void addWorst(LinkedList *f, LinkedList *u, string name, int count)
{
	double temp = (double)count/4 + 0.99999;
	int page = (int)temp;
	cout << "Here in AddWorst" << endl;
	ListNode* target = 0;
	ListNode* targetP = 0;
	ListNode* previous = f->getHead();
	ListNode* next = previous->getNext();
	int biggest = 0;
	do
	{
		if (next->getCount() > biggest && next->getName().compare("Free") == 0)
		{
			biggest = next->getCount();
			target = next;
			targetP = previous;
		}
		previous = next;
		next = next->getNext();
	} while (next != 0);

	if (biggest < page || target == 0)
		cout << "Error, Not enough memory for Program " << name << endl;
	else
	{
		ListNode* item;
		ListNode* last;
		ListNode* slot = u->findSlot(f, target);
				
		if (count > 8)
		{
			item = new ListNode(name, 4);
			count -= 4;
			last = new ListNode(name, 4);
			item->setNext(last);
			count -= 4;
			while (count > 4)
			{
				ListNode* t = new ListNode(name, 4);
				last->setNext(t);
				count -= 4;
				last = t;
			}
			ListNode* t2 = new ListNode(name, count);
			last->setNext(t2);
			last = t2;
		}
		else if (count > 4)
		{
			item = new ListNode(name, 4);
			count -= 4;
			ListNode* t3 = new ListNode(name, count);
			item->setNext(t3);
			last = t3;
		}
		else
		{
			item = new ListNode(name, count);
			last = item;
		}
		last->setNext(slot->getNext());
		slot->setNext(item);
		

		ListNode* split = new ListNode("Used", page);
		target->setCount(target->getCount() - page);
		split->setNext(target);
		targetP->setNext(split);

		//findTail(u->getHead())->setNext(item);
	}
	cout << "Return from AddWorst" << endl;
}

/* This function is used to find the correct ListNode for the insertion
   It is implemented by both the Best and Worst fit memory management methods
*/
ListNode* LinkedList::findSlot(LinkedList* f, ListNode* target)
{
	//cout << "Here in FindSlot" << endl;
	ListNode* result = getHead();
	ListNode* comp = f->getHead()->getNext();
	while (comp != 0 && comp != target)
	{
		//cout << "Slot while loop" << endl;
		if (comp->getName() != "Free")
		{
			int skip = comp->getCount();
			for (int i = 0; i < skip; i++)
			{
				//cout << "Slot for loop" << endl;
				result = result->getNext();
			}
		}
		comp = comp->getNext();
	}
	return result;
}

/* This function is called to remove (delete) an item ( ListNode ) 
   from the database 
   At the same time update both of the LinkedLists
   It is implemented by both the Best and Worst fit memory management methods
*/
void killProgram(LinkedList *f, LinkedList *u, string name)
{
	int pageSkip = 0;
	ListNode* previous = u->getHead();
	ListNode* find = previous->getNext();
	while (find != 0 && find->getName().compare(name) != 0)
	{
		previous = find;
		find = find->getNext();
		pageSkip++;
	}

	if (find != 0)
	{
		int pageCount = 1;

		ListNode* temp = find->getNext();

		
		while (temp != 0 && temp->getName().compare(name) == 0)
		{
			pageCount++;
			temp = temp->getNext();
		}
		

		previous->setNext(temp);


		int usedCount = 0;
		ListNode* previous2 = f->getHead();
		ListNode* find2 = previous2->getNext();
		
		do
		{
			if (find2->getName().compare("Used") == 0)
			{
				usedCount += find2->getCount();
			}
			if (pageSkip != 0)
			{
				previous2 = find2;
				find2 = find2->getNext();
			}
		} while (usedCount < pageSkip);

		while (find2->getName().compare("Free") == 0)
			find2 = find2->getNext();
		
		find2->setName("Free");

		mergeFree(f);
	}
	else
		cout << "Program " << name << " was not found in the memory. " << endl;
}

/* This function is discarded due to a better algorithm has been developed
   See the function "mergeFree"
*/
void defragmentation(ListNode* previous, ListNode* n2)
{
	int page = 0;
	if (previous != 0 && previous->getName().compare("Free") == 0)
		page += previous->getCount();
	while (n2->getName() == "Free")
	{
		page += n2->getCount();
		n2 = n2->getNext();
	}
	ListNode* newN = new ListNode("Free", page);
	previous->setNext(newN);
	newN->setNext(n2);
}

/* This function is called each time after a program is killed ( deleted )
   from the database. Its purpose is to merge the Free ListNodes, if there are
   any, from the Free list ( one of the LinkedLists )
*/
void mergeFree(LinkedList * f)
{
	int page = 0;
	ListNode* start = f->getHead();
	ListNode* end = start->getNext();
	while (end != 0)
	{
		if (end->getName().compare("Free") == 0)
		{
			page = 0;
			while (end != 0 && end->getName().compare("Free") == 0)
			{
				page += end->getCount();
				end = end->getNext();
			}
			ListNode* newOne = new ListNode("Free", page);
			start->setNext(newOne);
			newOne->setNext(end);
		}
		if (end != 0 && end->getName().compare("Used") == 0)
		{
			start = end;
		}
		if (end != 0)
			end = end->getNext();
	}
}

/* This function is called to print a chart-like representation of the items
   currently in the database 
   At the same time read from both of the LinkedLists
   It is implemented by both the Best and Worst fit memory management methods
*/
void printMem(LinkedList* f, LinkedList *u)
{
	int rowItem = 0;
	ListNode* trace = f->getHead()->getNext();
	ListNode* temp = u->getHead()->getNext();
	cout << endl;

	while (trace != 0)
	{
		if (trace->getName().compare("Free") == 0)
		{
			int p = trace->getCount();
			
			for (int i = 0; i < p; i++)
			{
				cout << "Free ";
				cout << "<" << p << "> ";
				rowItem++;
				if (rowItem == 8)
				{
					cout << endl;
					rowItem -= 8;
				}
			}
		}
		else if (trace->getName().compare("Used") == 0)
		{
			
			int p = trace->getCount();
			for (int i = 0; i < p; i++)
			{
				if (temp == 0)
					cout << "Null   ";
				else
				{
					cout << temp->getName() << "   ";
					cout << "(" << temp->getCount() << ")  ";
					temp = temp->getNext();
				}
				rowItem++;
				if (rowItem == 8)
				{
					cout << endl;
					rowItem -= 8;
				}
			}
		}
		trace = trace->getNext();
	}
}

/* This function is just for debugging purpose only. 
   Print the value of the ListNodes from any LinkedLists
*/
void printList(LinkedList* l)
{
	ListNode* t = l->getHead()->getNext();
	while (t != 0)
	{
		cout << t->getName() << "   ";
		t = t->getNext();
	}
	cout << endl << endl;

}

/* This function is called to traverse the Free list ( one of the LinkedLists)
to find the fragments in the database
It is implemented by both the Best and Worst fit memory management methods
*/
void fragments(LinkedList* f)
{
	int count = 0;
	ListNode* start = f->getHead()->getNext();

	while (start != 0)
	{
		if (start->getName().compare("Free") == 0)
			count++;
		start = start->getNext();
	}

	cout << "\nThere are " << count << " fragment(s)." << endl;
}


/* The following are wasted codes. No need to be curious to look at them.

void LinkedList::addNode(string name, int count)
{
ListNode* temp = getHead();
while (temp != 0)
temp = temp->getNext();
}

ListNode* LinkedList::findTail(ListNode* head)
{
ListNode* result = head;
while (result != 0)
result = result->getNext();
}
*/
