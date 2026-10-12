#pragma once
#define range 100
#define MAXSIZE 15000+100
#include <iostream>
#include <cstdlib>
#include <chrono>
using std::cout;
using std::endl;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
struct msClock
{
	typedef std::chrono::high_resolution_clock clock;
	std::chrono::time_point<clock> t1, t2;
	void Start() { t1 = high_resolution_clock::now(); }
	void Stop() { t2 = high_resolution_clock::now(); }
	double ElapsedTime()
	{
		duration<double, std::milli> ms_doubleC = t2-t1;
		return ms_doubleC.count();
	}
}
Clock;

int rand_pos(int x)
{
	if(x == 0)
		return 0;
	else
		return rand() % x;
}

class ArrayList
{
	int List[MAXSIZE];
	int Size;
public:
	ArrayList() {Size=0;}
	ArrayList(int s)
	{
		Size = 0;
		for (int i=0; i<s; i++)
			this->Insert(rand_pos(Size),rand_pos(range));
	}
	int size() {return Size;}
	int Insert(int p, int x)
	{
		//cout << "Insert(" << p << ", " << x << ")" << endl;
		if (Size==MAXSIZE)
			return -1;	// list full
		if (p<0 || p>Size)
			return -2;	// index out of bound
		for (int i=Size; i>p; i--)
			List[i] = List[i-1];
		List[p] = x;
		Size++;
		return 0;
	}
	int Delete(int p)
	{
		//cout << "Delete(" << p << ")" << endl;
		if (Size==0)
			return -1;	// list empty
		if (p<0 || p>Size-1)
			return -2;	// index out of bound
		for(int i = p; i<Size-1; i++)
			List[i] = List[i+1];
		Size--;
		return 0;
	}
	int Retrieve(int p)
	{
		if (p<0 || p>Size)
		{
			cout << "ERROR: Out of bound p: " << p << ", Size: " << Size << endl;
			exit(-2);
			return -2;		// Out of bound
		}
		else
		{
			return List[p];
		}
	}
	void Display()
	{
		for (int i=0; i<Size; i++)
			cout << List[i] << " ";
		cout << endl;
	}
};

struct Node
{
	int Data;
	Node* Next;
};
class LinkedList
{
	Node* First;
	int Size;
public:
	LinkedList()
	{
		First = NULL;
		Size = 0;
	}
	LinkedList(int s)
	{
		First = NULL;
		Size = 0;
		for (int i=0; i<s; i++)
			this->Insert(rand_pos(Size),rand_pos(range));
	}
	int size() {return Size;}
	int Retrieve(int p)
	{
		if (p<0 || p>Size)
		{
			cout << "ERROR: Out of bound p: " << p << ", Size: " << Size << endl;
			exit(-2);
			return -2;		// Out of bound
		}
		else
		{
			Node* current = First;
			for (int i=0; i<p; i++)
				current = current->Next;
			return current->Data;
		}
	}
	void InsertatHead(int x)
	{
		Node* newNode = new Node;
		newNode->Next = First;
		newNode->Data = x;
		First = newNode;
		Size++;
	}
	int Insert(int p, int x)
	{
		//cout << "Insert(" << p << ", " << x << ")" << endl;
		if (p<0 || p>Size)
			return -2;		// Out of bound
		else
		{
			Node* newNode = new Node;
			newNode->Data = x;
			if (p==0)	// Insertion at head or empty list
			{
				newNode->Next = First;
				First = newNode;
			}
			else	// else we traverse the list
			{
				Node* prev = First;
				for (int i=0; i<p-1; i++)
					prev = prev->Next;
				newNode->Next = prev->Next;
				prev->Next = newNode;
			}
			Size++;
			return 0;
		}
	}
	int Delete(int p)
	{
		//cout << "Delete(" << p << ")" << endl;
		if (First==NULL)	// empty list
			return -1;
		if (p<0 || p>=Size)
			return -2;		// Out of bound
		if (p==0)	// first node
		{
			Node* temp = First;
			First = First->Next;
			delete temp;
		}
		else
		{
			Node* prev = First;
			for (int i=0; i<p-1; i++)
				prev = prev->Next;
			Node* temp = prev->Next;
			prev->Next = prev->Next->Next;
			delete temp;
		}
		Size--;
		return 0;
	}
	void Display()
	{
		Node* current = First;
		while (current)
		{
			cout << current->Data << " ";
			current = current->Next;
		}
		cout << endl;
	}
	~LinkedList()
	{
		Node* current = First;
		Node* next = NULL;
		while (current)
		{
			next = current->Next;
			delete current;
			current = next;
		}
	}
};
