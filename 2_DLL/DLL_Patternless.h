#pragma once

#ifdef _DEBUG
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif

#include <iostream>

template<class T>
class LinkedList {
private:
	struct Node {
		Node* m_fwd;
		Node* m_bwd;
		T m_value;

		Node(const T& value, Node* fwd, Node* bwd) : m_value(value), m_fwd(fwd), m_bwd(bwd) { }
	};

	Node* m_first;
	Node* m_last;
	size_t m_count;
public:
	LinkedList() {
		m_first = m_last = nullptr;
		m_count = 0;
	}

	virtual ~LinkedList() {
		clear();
	}

	const size_t count() inline const { return m_count; }

	// insert element at the back
	void push_back(const T& value) {
		if (m_last) {
			m_last->m_bwd = new Node(value, m_last, nullptr);
			m_last = m_last->m_bwd;
		}
		else
			m_first = m_last = new Node(value, nullptr, nullptr);

		++m_count;
	}

	// insert element at front back
	void push_front(const T& value) {
		if (m_first) {
			m_first->m_fwd = new Node(value, nullptr, m_first);
			m_first = m_first->m_fwd;
		}
		else
			m_first = m_last = new Node(value, nullptr, nullptr);

		++m_count;
	}

	// returns front element
	const T& peek_front() const {
		return m_first ? m_first->m_value : 0;
	}

	// returns back element
	const T& peek_back() const {
		return m_last ? m_last->m_value : 0;
	}

	// inserts after given index
	void insert_before(const T& value, const size_t index) {
		if (index >= 0 && index <= m_count) {
			const T& lastValue = m_last->m_value;
			if (index - 1 > m_count / 2) {
				Node* curr = m_last;
				for (size_t i = 0; i < m_count - index; ++i)
					curr = curr->m_fwd;
				Node* newValue = new Node(value, curr, curr->m_bwd);
				curr->m_bwd->m_fwd = newValue;
				curr->m_bwd = newValue;
			}
			else {
				Node* curr = m_first;
				for (size_t i = 0; i < index; ++i)
					curr = curr->m_bwd;
				Node* newValue = new Node(value, curr->m_fwd, curr);
				curr->m_fwd->m_bwd = newValue;
				curr->m_fwd = newValue;
			}
			push_back(lastValue);
		}
	}

	// inserts after given index
	void insert_after(const T& value, const size_t index) {
		if (index >= 0 && index <= m_count) {
			const T& lastValue = m_last->m_value;
			if (index - 1 > m_count / 2) {
				Node* curr = m_last;
				for (size_t i = 0; i < m_count - index; ++i)
					curr = curr->m_fwd;
				Node* newValue = new Node(value, curr->m_fwd, curr);
				curr->m_fwd->m_bwd = newValue;
				curr->m_fwd = newValue;
			}
			else {
				Node* curr = m_first;
				for (size_t i = 0; i < index; ++i)
					curr = curr->m_bwd;
				Node* newValue = new Node(value, curr, curr->m_bwd);
				curr->m_bwd->m_fwd = newValue;
				curr->m_bwd = newValue;
			}
			push_back(lastValue);
		}
	}

	// pops and returns front element
	const T& pop_front() {
		T result = 0;
		if (m_first) {

			result = m_first->m_value;
			Node* node = m_first;
			m_first = m_first->m_bwd;
			delete node;

			if (!--m_count)
				m_last = nullptr;
		}
		return result;
	}

	// pops and returns back element
	const T& pop_back() {
		T result = 0;
		if (m_last) {

			result = m_last->m_value;
			Node* node = m_last;
			m_last = m_last->m_fwd;
			delete node;

			if (!--m_count)
				m_first = nullptr;
		}
		return result;
	}

	// clears list
	void clear() {
		while (m_last)
			pop_front();
	}
};