#ifdef _DEBUG 
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#endif

#include <iostream>
#include "DLL.h"
#include <assert.h>

class Node : public Link<Node> {
public:
	float data;

	Node(float v = 0) :data(v) {}
	virtual std::ostream& Print(std::ostream& cout) const {
		return cout << data;
	}
	bool operator==(const Node& rhs) const {
		return data == rhs.data;
	}
};

template Link<Node>;
template List<Node>;


void TestDLL() {
	List<Node> myList;
	assert(myList.Check());
	Node * nodeA3 = myList.InsertFirst(new Node(3.f));
	assert(myList.Check());
	myList.InsertFirst(new Node(2.f));
	myList.InsertFirst(new Node(1.f));
	assert(myList.Check());
	myList.InsertLast(new Node(1.f));
	myList.InsertLast(new Node(2.f));
	myList.InsertLast(new Node(3.f));
	assert(myList.Check());
	assert(myList.Last()->Next() == nullptr);
	std::cout << myList << std::endl;    //should be 1 2 3 1 2 3

	Node * tempA3 = myList.FindNext(3.f);
	assert(tempA3 == nodeA3);
	Node * nodeB1 = nodeA3->FindNext(1.f);
	Node * tempB1 = tempA3->DeleteAfter();   //ta bort andra 1:an
	assert(myList.Check());
	assert(tempB1->data == 1);

	Node * nodeA2 = myList.FindFirst(2.f);
	Node * nodeB2 = nodeA2->FindNext(2.f);
	Node * temp = nodeB2->FindNext(2.f);
	assert(!temp);

	nodeA2->DeleteAfter();
	std::cout << myList << std::endl;    //1 2 2 3

	myList.First()->Next()->InsertAfter(tempA3)->InsertAfter(tempB1);
	assert(myList.Last()->Prev()->data == 2.f);
	assert(myList.Check());
	std::cout << myList << std::endl;    //should be 1 2 3 1 2 3
	{Node* t = myList.PopFirst(); assert(t->data == 1.f); delete t; }
	{Node* t = myList.PopFirst(); assert(t->data == 2.f); delete t; }
	{Node* t = myList.PopFirst(); assert(t->data == 3.f); delete t; }
	{Node* t = myList.PopFirst(); assert(t->data == 1.f); delete t; }
	{Node* t = myList.PopFirst(); assert(t->data == 2.f); delete t; }
	{Node* t = myList.PopFirst(); assert(t->data == 3.f); delete t; }
	assert(myList.PopFirst() == nullptr);
	assert(myList.PopFirst() == nullptr);
	std::cout << myList << "end" << std::endl;
	assert(myList.Check());
	myList.InsertFirst(new Node(2));
	std::cin.get();
}


int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	TestDLL();

	return EXIT_SUCCESS;
}