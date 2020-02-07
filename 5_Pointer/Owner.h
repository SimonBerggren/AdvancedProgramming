#pragma once

struct Owner {
	Owner() = default;
	size_t num_weak = 0;
	size_t num_strong = 0;
	inline void inc_weak() { ++num_weak; }
	inline void dec_weak() { --num_weak; }
	inline void inc_strong() { ++num_strong; }
	inline void dec_strong() { --num_strong; }
	inline bool expired() const { return num_strong == 0 && num_weak == 0; }
	inline bool no_owner() const { return num_strong == 0; }
};