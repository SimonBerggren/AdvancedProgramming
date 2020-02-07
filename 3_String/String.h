#pragma once

#include <iostream>
#include <string>
#include "Iterator.h"
#include <Windows.h>

class String
{
private:
	char* m_buffer;				// character buffer
	size_t m_count;				// character count
	size_t m_capacity;			// maximum characters

public:
	~String();					// destructor
	String();					// constructor
	String(const String& rhs);	// copy-constructor
	String(String&& rhs);		// move-constructor
	String(const char* rhs);	// assignment-constructor

	void push_back(const char c);							// push char to end
	const size_t capacity() const { return m_capacity; }	// maximum size
	const size_t count() const { return m_count; }			// current size
	const char* cstr() const;								// to c-style string const
	char* cstr();											// to c-style strings
	const char& at(const size_t i) const;					// index with range check const
	char& at(const size_t i);								// index with range check
	void resize(const size_t i);							// resizes capacity, characters outside bounds are lost
	void shrink_to_fit();									// resizes capacity to fit count
	void reserve(const size_t i);							// reserves additional memory

	const char* data() const { return m_buffer; }
	operator bool() const;

	typedef IteratorBase<char, iterator<char>, 1> const_iterator;
	typedef IteratorBase<char, iterator<char, -1>, -1> const_reverse_iterator;
	typedef iterator<char, -1> reverse_iterator;
	typedef iterator<char> iterator;

	iterator begin() { return iterator(m_buffer); }
	iterator end() { return iterator(m_buffer + m_count); }
	const_iterator cbegin() { return const_iterator(m_buffer); }
	const_iterator cend() { return const_iterator(m_buffer + m_count); }
	reverse_iterator rbegin() { return reverse_iterator(m_buffer + m_count - 1); }
	reverse_iterator rend() { return reverse_iterator(m_buffer - 1); }
	const_reverse_iterator crbegin() { return const_reverse_iterator(m_buffer + m_count - 1); }
	const_reverse_iterator crend() { return const_reverse_iterator(m_buffer - 1); }

	// operators
	friend std::ostream& operator<<(std::ostream& cout, const String& rhs);		// << out
	friend bool operator==(const String& lhs, const String& rhs);				// ==
	friend bool operator!=(const String& lhs, const String& rhs);				// !=
	const char& operator[](const size_t i) const;								// [] const
	char& operator[](const size_t i);											// []
	String& operator=(const String& rhs);										// =
	String& operator=(String&& rhs);											// = move
	String& operator=(const char* rhs);											// = char
	String& operator=(const std::string& rhs);									// = string
	String& operator+=(const String& rhs);										// +=
	String& operator+=(const char* rhs);										// += char
	String& operator+=(const std::string& rhs);									// += string
	String& operator+=(const char rhs);											// += char
	String& operator+(const char rhs);											// + char
	String& operator+(const char* rhs);											// + string
	String& operator+(const String& rhs);										// + char

																				// UNIMPLEMENTED //
	void join(const char* str, const char* separator);		// concatenates using separator
	void replace(const char* str, const char* replacer);	// find and replace
	const char* to_upper() const;							// to upper case const
	char* to_upper();										// to upper case
	const char* to_lower() const;							// to lower case const
	char* to_lower();										// to lower case

private:
	void assign(const size_t i, const char* rhs);
};
