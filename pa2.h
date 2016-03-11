#ifndef PA2_H
#define PA2_H
#include <string>
#include <iostream>
using namespace std;

class ListNode
{
private:
	string m_name;
	int m_occupy;
	ListNode *next;
public:
	ListNode(string name, int count = 0) : m_name( name ), m_occupy( count )
	{
		next = 0;
	}

	string getName() { return m_name; }
	void setName(string name) { m_name = name; }
	int getCount() { return m_occupy; }
	void setCount(int count) { m_occupy = count; }

	ListNode* getNext() { return next; }
	void setNext(ListNode* nextN) { next = nextN; }
};

class LinkedList
{
private: 
	ListNode *head;
	
public:
	LinkedList() 
	{
		head = new ListNode("Head", 0);
	}

	LinkedList(string placeHolder)  
	{
		head = new ListNode("Head", 0);
		head->setNext(new ListNode(placeHolder, 32));
	}

	ListNode* getHead(){ return head; }
	void addNode(string name, int count);

	

	void addBest(LinkedList *f, LinkedList *u, string name, int count);
	
	
	
	void fragmentation();
	
	

	ListNode* findTail(ListNode* head);
	ListNode* findSlot(LinkedList* f, ListNode* target);
};

void printMenu();
void defragmentation(ListNode* previous, ListNode* n2);
void mergeFree(LinkedList* f);
void printList(LinkedList* l);

void processBest(int choice, LinkedList *f, LinkedList *u);
void processWorst(int choice, LinkedList *f, LinkedList *u);

void addBest(LinkedList *f, LinkedList *u, string name, int count);
void addWorst(LinkedList *f, LinkedList *u, string name, int count);

void killProgram(LinkedList *f, LinkedList *u, string name);
void printMem(LinkedList* f, LinkedList *u);
void fragments(LinkedList* f);

#endif


/*
class TrackingNode
{
private:
int pageCount;
public:
TrackingNode(int page = 32) : pageCount{ page }
{

}
};
*/

/*class LinkedList
{
private:
int m_pageCount;
const int m_pageLimit;
ListNode* head;
public:
LinkedList(int pL = 32) : m_pageLimit{pL}
{
head = new ListNode("Free", 0);
}

ListNode *getHead(){ return head; }
void setHead(ListNode * newH){ head = newH; }
};
*/