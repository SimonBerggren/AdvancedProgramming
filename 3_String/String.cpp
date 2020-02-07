#define _CRTDBG_MAP_ALLOC
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#include "String.h"

String::~String()
{
	if (m_buffer)
		delete[] m_buffer;
}

String::String() : m_buffer(nullptr)
{
	assign(0, m_buffer);
}

String::String(const String& rhs) : m_buffer(nullptr)
{
	*this = rhs;
}

String::String(String&& rhs) : m_buffer(nullptr)
{
	*this = std::move(rhs);
}

String::String(const char* rhs) : m_buffer(nullptr)
{
	*this = rhs;
}

void String::push_back(const char c)
{
	if (++m_count > m_capacity)
		resize(m_count * 2);

	m_buffer[m_count - 1] = c;
	m_buffer[m_count] = '\0';
}

const char* String::cstr() const
{
	return m_buffer;
}

char* String::cstr()
{
	return m_buffer;
}

const char& String::at(const size_t i) const
{
	if (i >= m_count)
		throw std::out_of_range("Index out of range!");

	return m_buffer[i];
}

char& String::at(const size_t i)
{
	if (i >= m_count)
		throw std::out_of_range("Index out of range!");

	return m_buffer[i];
}

void String::resize(const size_t size)
{
	if (m_capacity != size) {
		char* new_buffer = new char[size + 1];
		new_buffer[size] = '\0';
		memcpy(new_buffer, m_buffer, size);

		if (m_buffer)
			delete[] m_buffer;
		m_buffer = new_buffer;
		m_capacity = size;
		if (m_count > size)
			m_count = size;
	}
}

void String::shrink_to_fit()
{
	resize(m_count);
}

void String::reserve(const size_t i)
{
	if (i > m_capacity)
		resize(i);
}

String::operator bool() const
{
	return m_count != 0;
}

const char& String::operator[](const size_t i) const
{
	return m_buffer[i];
}

char& String::operator[](const size_t i)
{
	return m_buffer[i];
}

String& String::operator=(const String& rhs)
{
	return *this = rhs.cstr();
}

String& String::operator=(String&& rhs)
{
	if (m_buffer)
		delete[] m_buffer;

	m_buffer = rhs.m_buffer;
	m_count = rhs.m_count;
	m_capacity = rhs.m_capacity;

	rhs.m_buffer = nullptr;
	rhs.m_count = 0;
	rhs.m_capacity = 0;
	return *this;
}

String& String::operator=(const char* rhs)
{
	if (m_buffer != rhs)
		assign(strlen(rhs), rhs);
	return *this;
}

String& String::operator=(const std::string& rhs)
{
	return *this = rhs.c_str();
}

String& String::operator+=(const String& rhs)
{
	return *this += rhs.cstr();
}

String& String::operator+=(const char* rhs)
{
	// don't += ourselves
	if (rhs != m_buffer)
	{
		const size_t len = strlen(rhs);
		for (auto c = rhs; c != rhs + len; ++c)
			push_back(*c);
	}

	return *this;
}

String& String::operator+=(const std::string& rhs)
{
	return *this += rhs.c_str();
}

String& String::operator+=(const char rhs)
{
	push_back(rhs);
	return *this;
}

String& String::operator+(const char rhs)
{
	String s = *this;
	return s += rhs;
}

String & String::operator+(const char * rhs)
{
	String s = *this;
	return s += rhs;
}

String& String::operator+(const String & rhs)
{
	String s = *this;
	return s += rhs;
}

std::ostream& operator<<(std::ostream& cout, const String& rhs)
{
	for (size_t i = 0; i < rhs.m_count; ++i)
		cout << rhs[i];

	return cout;
}

bool operator==(const String& lhs, const String& rhs)
{
	if (&lhs != &rhs && lhs.m_count == rhs.m_count)
		for (size_t i = 0; i < rhs.m_count; ++i)
			if (lhs[i] != rhs[i])
				return false;

	return true;
}

bool operator!=(const String& lhs, const String& rhs)
{
	return !(lhs == rhs);
}

void String::assign(const size_t size, const char* rhs)
{
	char* new_buffer = new char[size + 1];
	new_buffer[size] = '\0';
	memcpy(new_buffer, rhs, size);

		delete[] m_buffer;
	m_buffer = new_buffer;
	m_count = m_capacity = size;
}