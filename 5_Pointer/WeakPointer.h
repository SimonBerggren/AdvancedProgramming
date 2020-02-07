#pragma once

#include "SharedPointer.h"
#include "Owner.h"

template <class T>
class WeakPointer {
	template <class I>
	friend class SharedPointer;
	Owner* m_owner;
	T* m_value;
	void inc_weak() { if (m_value) m_owner->inc_weak(); }
	void reset(T* value = nullptr, Owner* owner = nullptr) {
		if (m_owner) {
			m_owner->dec_weak();
			if (m_owner->expired()) {
				delete m_owner;
				m_owner = nullptr;
			}
		}
		m_value = value;
		m_owner = owner;
	}
public:
	~WeakPointer() { reset(); }
	WeakPointer() : m_value(nullptr), m_owner(nullptr) { }
	WeakPointer(const WeakPointer& rhs) {
		*this = rhs;
	}
	template <class O>
	WeakPointer(const SharedPointer<O>& rhs) {
		*this = rhs;
	}
	bool expired() const {
		return !m_owner || m_owner && m_owner->no_owner();
	}
	SharedPointer<T> lock() { 
		return SharedPointer<T>(m_value, m_owner); 
	}
	template <class O>
	bool operator==(const WeakPointer<O>& rhs) const { return m_value == rhs.m_value; }
	template <class O>
	bool operator!=(const WeakPointer<O>& rhs) const { return m_value != rhs.m_value; }
	template <class O>
	WeakPointer& operator=(const SharedPointer<O>& rhs) {
		reset(rhs.m_value, rhs.m_owner);
		inc_weak();
		return *this;
	}
	WeakPointer& operator=(const WeakPointer& rhs) {
		if (rhs != *this) {
			reset(rhs.m_value, rhs.m_owner);
			inc_weak();
		}
		return *this;
	}
};