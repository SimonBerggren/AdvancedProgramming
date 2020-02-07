#pragma once

#include <iostream>
#include <assert.h>
#include "Link.h"

template<class T>
class List : public Link<T> {
public:
	T* First() { return dynamic_cast<T*>(next); }
	T* Last() { return dynamic_cast<T*>(prev); }
	virtual ~List() {
		Link<T>* r = next;
		while (r != this) {
			auto rn = r->next;
			delete r;
			r = rn;
		}
	}
	List() {
		next = prev = this;
	}

	T* InsertFirst(T* node) {
		return InsertAfter(node);
	}

	T* InsertLast(T* node) {
		return InsertBefore(node);
	}

	template <class S>
	T* FindFirst(const S& searchFor) {
		return next->FindNext(searchFor);
	}

	T* PopFirst() {
		return DeleteAfter();
	}

	void print(std::ostream& os) {
		const Link<T>* node = this;
		const Link<T>* nextNode = next;
		do {
			node = nextNode;
			nextNode = nextNode->next;
			node->Print(os);
		} while (node != this);
	}

	bool Check() {
		const Link<T>* node = this;
		const Link<T>* nextNode = next;
		do {
			assert(node->next == nextNode && nextNode->prev == node);
			node = nextNode;
			nextNode = nextNode->next;
		} while (node != this);
		return true;
	}

	friend std::ostream& operator<<(std::ostream& cout, List<T>& list) {
		list.print(cout);
		return cout;
	}
};