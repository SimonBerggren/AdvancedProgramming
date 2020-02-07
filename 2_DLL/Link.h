#pragma once

template<class T>
class Link {
public:
	Link* next = nullptr;
	Link* prev = nullptr;
	Link() : next(nullptr), prev(nullptr) { }
	virtual ~Link() = default;

	T* Next() { return dynamic_cast<T*>(next); }
	T* Prev() { return dynamic_cast<T*>(prev); }
	const T* Next() const { return dynamic_cast<T*>(next); }
	const T* Prev() const { return dynamic_cast<T*>(prev); }

	T* InsertAfter(T* TToInsert) {
		TToInsert->prev = this;
		TToInsert->next = next;
		next->prev = TToInsert;
		next = TToInsert;
		return TToInsert;
	}
	T* InsertBefore(T* TToInsert) {
		TToInsert->prev = prev;
		TToInsert->next = this;
		prev->next = TToInsert;
		prev = TToInsert;
		return TToInsert;
	}
	T* DeleteAfter() {
		if (T* r = Next()) {
			next = next->next;
			next->prev = r->prev;
			return r;
		}
		return nullptr;
	}
	template <class S>
	T* FindNext(const S& searchFor) {
		for (T* it = Next(); it; it = it->Next())
			if (*it == searchFor)
				return it;

		return nullptr;
	}
	virtual std::ostream& Print(std::ostream& cout) const {
		return cout;
	}
};