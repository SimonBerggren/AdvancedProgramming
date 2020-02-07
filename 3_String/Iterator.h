#pragma once

#include <iterator>

template <class T, class IT, char step>
class IteratorBase : public std::iterator<std::random_access_iterator_tag, T> {
protected:
	T* m_value;
public:
	IteratorBase() : m_value(nullptr) { }
	IteratorBase(T* v) : m_value(v) { }
	IteratorBase(const IT& rhs) { *this = rhs; }
	const T& operator*() const { return *m_value; }
	const T& operator[](const int i) const { return *(m_value + i * step); }
	bool operator==(const IteratorBase& rhs) const { return m_value == rhs.m_value; }
	bool operator!=(const IteratorBase& rhs) const { return m_value != rhs.m_value; }
	bool operator<(const IteratorBase& rhs) const { return step > 0 ? m_value < rhs.m_value : m_value > rhs.m_value; }
	bool operator>(const IteratorBase& rhs) const { return step > 0 ? m_value > rhs.m_value : m_value < rhs.m_value; }
	IT& operator=(const IT& rhs) { m_value = rhs.m_value; step = rhs.step; return static_cast<IT&>(*this); }
	IT& operator+=(const int i) { m_value += step*i; return static_cast<IT&>(*this); }
	IT operator++(int) { auto i = *this; m_value += step; return static_cast<IT&>(i); }
	IT operator--(int) { auto i = *this; m_value -= step; return static_cast<IT&>(i); }
	IT& operator++() { m_value += step; return static_cast<IT&>(*this); }
	IT& operator--() { m_value -= step; return static_cast<IT&>(*this); }
	int operator-(const IT& rhs) const { return m_value - rhs.m_value; }
	IT operator+(const int i) const { return IT(m_value + i * step); }
	IT operator-(const int i) const { return IT(m_value - i * step); }
};

template <class T, char step = 1>
struct iterator : public IteratorBase<T, iterator<T, step>, step> {
	using IteratorBase::IteratorBase;
	T& operator*() { return *m_value; }
	T& operator[](int i) { return *(m_value + i * step); }
};