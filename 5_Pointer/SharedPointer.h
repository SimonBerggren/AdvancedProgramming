#pragma once

#include "Owner.h"
#include <memory>

template <class I> class WeakPointer;

template <class T>
class SharedPointer {
	template <class I>
	friend class WeakPointer;
	T* m_value;
	Owner* m_owner;
	inline void inc_strong() { if (m_owner) m_owner->inc_strong(); };
public:
	~SharedPointer() { reset(); }
	SharedPointer() : m_value(nullptr), m_owner(nullptr) { }
	SharedPointer(T* value, Owner* owner = nullptr)
		: m_value(value), m_owner(nullptr) {
		if (value) {
			m_owner = new Owner();
			inc_strong();
		}
	}
	SharedPointer(const SharedPointer& rhs)
		: m_value(nullptr), m_owner(nullptr) {
		reset(rhs.m_value, rhs.m_owner);
		inc_strong();
	}
	template <class O>
	SharedPointer(const WeakPointer<O>& rhs)
		: m_value(nullptr), m_owner(nullptr) {
		if (rhs.expired())
			throw "std::bad_weak_ptr";
		reset(rhs.m_value, rhs.m_owner);
		inc_strong();
	}
	SharedPointer(SharedPointer&& rhs)
		: m_value(nullptr), m_owner(nullptr) {
		*this = std::move(rhs);
	}
	const bool unique() const { return m_owner && m_owner->num_strong == 1; }
	T const* operator->() const { return m_value; }
	T const* get() { return m_value; }

	T& operator*() const { return *m_value; }
	operator bool() const { return m_value != nullptr; }

	bool operator==(const T* value) const { return m_value == value; }
	bool operator<(const SharedPointer& rhs) const { return m_value < rhs.m_value; }
	bool operator==(const nullptr_t value) const { return m_value == value; }
	bool operator==(const SharedPointer& rhs) const { return m_value == rhs.m_value && m_owner == rhs.m_owner; }

	SharedPointer& operator=(const SharedPointer& rhs) {
		if (rhs != *this) {
			reset(rhs.m_value, rhs.m_owner);
			inc_strong();
		} return *this;
	}
	SharedPointer& operator=(SharedPointer&& rhs) {
		if (&rhs != this) {
			reset(rhs.m_value, rhs.m_owner);
			rhs.m_value = nullptr;
			rhs.m_owner = nullptr;
		} return *this;
	}
	template <class O>
	SharedPointer& operator=(const WeakPointer<O>& rhs) {
		reset(rhs.m_value, rhs.m_owner);
		inc_strong();
		return *this;
	}
	void reset(T* value = nullptr, Owner* owner = nullptr) {
		if (m_owner) {
			m_owner->dec_strong();
			if (m_owner->no_owner())
				delete m_value;
			if (m_owner->expired())
				delete m_owner;
			m_owner = nullptr;
			m_value = nullptr;
		}
		m_value = value;
		m_owner = owner;
	}
};