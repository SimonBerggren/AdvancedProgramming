#pragma once

#include "GCD.h"
#include <iostream>

template <typename T, typename R> bool operator!=(const T& lhs, const R& rhs) { return !(lhs == rhs); }
template <typename T, typename R> bool operator>(const T& lhs, const R& rhs) { return (rhs < lhs); }
template <typename T, typename R> bool operator>=(const T& lhs, const R& rhs) { return !(lhs < rhs); }
template <typename T, typename R> bool operator<=(const T& lhs, const R& rhs) { return !(rhs < lhs); }

#define RT(T, R) typename largest_type<T, R>::return_type

template <class T, class R> struct largest_type {
	using value_type = T;
	using return_type = T;
};

template <> struct largest_type<int, int> {
	using value_type = long long;
	using return_type = int;
};

template <> struct largest_type<int, short> {
	using value_type = long long;
	using return_type = int;
};

template <> struct largest_type<short, int> {
	using value_type = long long;
	using return_type = int;
};

template <> struct largest_type<short, short> {
	using value_type = int;
	using return_type = short;
};

template <> struct largest_type<short, long long> {
	using value_type = long long;
	using return_type = long long;
};

template <> struct largest_type<long long, short> {
	using value_type = long long;
	using return_type = long long;
};

template <class T>
class Rational {
public:
	T nom, denom;

	// constructors
	Rational() : nom(0), denom(1) {}
	Rational(T P) :nom(P), denom(1) {}
	Rational(T P, T Q) :nom(P), denom(Q) { Reduce(P, Q); }
	template<class T> Rational(const Rational<T>& R) : nom(R.nom), denom(R.denom) { }

	// comparison operators
	template<class T, class R> friend bool operator==(const Rational<T>&, const Rational<R>&);
	template<class T, class R> friend bool operator==(const T&, const Rational<R>&);
	template<class T, class R> friend bool operator==(const Rational<T>&, const R&);
	template<class T>bool operator<(const Rational<T>& rhs) const { return nom / denom < rhs.nom / rhs.denom; }
	template<class T>bool operator<(const T& rhs) const { return nom / denom < rhs; }

	// + operators
	template<class T, class R> friend Rational<RT(T, R)> operator+(const T&, const Rational<R>&);
	template<class T, class R> friend Rational<RT(T, R)> operator+(const Rational<T>&, const R&);
	template<class T, class R> friend Rational<RT(T, R)> operator+(const Rational<T>&, const Rational<R>&);

	// += operator
	template<class T> Rational<T> operator+=(const T& rhs) { nom = nom + rhs * denom; return *this; }

	// - unary operator
	Rational<T>& operator-() const { return Rational<T>(-nom, denom); }

	// ++ operators
	Rational<T>& operator++() { nom += denom; return *this; }
	Rational<T>& operator++(int) { auto n = Rational(nom, denom); nom += denom; return n; }
	
	// convertion operators
	explicit operator long long() const { return long long(nom / denom); }
	explicit operator int() const { return int(nom / denom); }
	explicit operator short() const { return short(nom / denom); }

	// << >> operators
	template<class T> friend std::ostream& operator<<(std::ostream& cout, Rational<T>& R);
	template<class T> friend std::istream& operator>>(std::istream& cin, Rational<T>& R);
};

// ==
template<class T, class R>
bool operator==(const Rational<T>& lhs, const R& rhs) {
	return lhs.nom == rhs && lhs.denom == 1;
}

// ==
template<class T, class R>
bool operator==(const T& lhs, const Rational<R>& rhs) {
	return rhs.nom == lhs && rhs.denom == 1;
}

// ==
template<class T, class R>
bool operator==(const Rational<T>& lhs, const Rational<R>& rhs) {
	return lhs.nom / lhs.denom == rhs.nom / rhs.denom;
}

// <<
template<class T>
std::ostream& operator<< (std::ostream& cout, Rational<T>& R) {
	cout << R.nom << '/' << R.denom;
	return cout;
}

// >>
template<class T>
std::istream& operator>> (std::istream& cin, Rational<T>& R) {
	char snestreck;
	cin >> R.nom >> snestreck >> R.denom;
	return cin;
}

// +
template<class T, class R>
Rational <RT(T, R)> operator+(const T& lhs, const Rational<R>& rhs) {
	return Rational<R>(lhs) + rhs;
}

// +
template<class T, class R>
Rational <RT(T, R)> operator+(const Rational<T>& lhs, const R& rhs) {
	return lhs + Rational<R>(rhs);
}

// +
template<class T, class R>
Rational <RT(T, R)> operator+(const Rational<T>& lhs, const Rational<R>& rhs) {

	// a/b + c/d  =  a*d + c*b / b*d

	typedef largest_type<T, R>::value_type t;

	auto a = t(lhs.nom);
	auto b = t(lhs.denom);
	auto c = t(rhs.nom);
	auto d = t(rhs.denom);

	auto p = a * d + c * b;
	auto q = b * d;

	return Rational<RT(T, R)>(p, q);
}