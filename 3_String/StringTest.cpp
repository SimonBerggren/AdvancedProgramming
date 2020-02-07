#define _CRTDBG_MAP_ALLOC
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#include <stdlib.h>
#include <crtdbg.h>
#include "String.h"
#include <iostream>
#include <cassert>
#include"StringTest.h"
#include <algorithm>

using namespace std;
using std::cout;
using std::cin;

void TestPushBackReallocation() {
    String str("hej");
    assert(str.count() <= str.capacity());

#ifdef VG
    //If VG we try to take 20 empty places: (count+1 < capacity)
    //we push_back one more until it is more then 20 places left (or sting to big)
    while (str.count() + 20 >= str.capacity() && str.count() < 1000)
        str.push_back('A' + rand() % 32);
    assert(str.count() < 1000);	//If this fail it prbably the case that capacity is increased with a constant.
#endif //VG

    auto internalBuf = str.data();
    auto cap = str.capacity();
    auto siz = str.count();
    size_t i;
    for (i = siz + 1; i <= cap; ++i) {
        str.push_back(char(i) + 'a');
        assert(internalBuf == str.data());
        assert(cap == str.capacity());
        assert(i == str.count());
    }
    str.push_back(char(i));
    assert(internalBuf != str.data());
    assert(cap < str.capacity());
    assert(i == str.count());

    assert(str.count() != str.capacity());
    internalBuf = str.data();
    cap = str.capacity();
    siz = str.count();
    str.shrink_to_fit();
    assert(internalBuf != str.data());
    assert(str.count() == str.capacity());
    assert(i == str.count());
}

void TestFörGodkäntString() {
    //-	String()
    String str0;
    String str00("");
    assert(str0 == str00);
    str0 != str00;
    //-	String(Sträng sträng)
    String s1("foo"); assert(s1 == "foo");
    String str(s1); assert(str == "foo");
    String s3("bar");  assert(s3 == "bar");

    //-	~String() Kom ihåg destruktorn!
    delete new String("hej");

    //	-	operator =(Sträng sträng)
    assert((str = s3) == s3);
    assert((str = str) == s3);	//self assignment


    String str1("foo"), str2("bar"), str3("hej");
    str3 = str = str1;
    assert(str3 == str);
    assert(str1 == str);

    // operator bool

    assert(String("huj"));
    assert(!String(""));

    //-	operator== 
    //testas överallt!


    //-	operator[](int i) som indexerar utan range check.
    str = "bar";
    str[-1]; str[1000];	//No error
    assert(str[1] == 'a');
    str[1] = 'y';
    assert(str[1] == 'y');

    const String sc(str);
    assert(sc[1] == 'y');
    assert(std::is_const<std::remove_reference< decltype(sc[1])>::type>::value); //Kolla att det blir en const resultat av indexering

                                                                                 //-	push_back(char c) lägger till ett tecken sist.
    str = "bar";
    str.push_back('a');
    assert(str == "bara");

    //-	count(), capacity() and reloccation test;
    TestPushBackReallocation();

    cout << String("hej\n");
    cout << "Om det står hej på föregående rad så är TestFörGodkänt klar\n";

}

#ifdef VG

void TestFörVälGodkäntString() {
    String str("bar");

    //-	at(int i) som indexerar med range check
    try {
        str.at(-1);
        assert(false);
    }
    catch (std::out_of_range&) {};
    try {
        str.at(3);
        assert(false);
    }
    catch (std::out_of_range&) {};

    //- at indexerar

    //-	at(int i) 
    str = "bar";
    assert(str.at(1) == 'a');
    str.at(1) = 'y';
    assert(str.at(1) == 'y');

    const String strC(str);
    assert(strC.at(1) == 'y');
    assert(std::is_const<std::remove_reference< decltype(strC.at(1))>::type>::value); //Kolla att det blir en const resultat av indexering

                                                                                      // ConvertToChars
    const char* temp = strC.data();
    assert(strC.count() == 3);
    assert(std::memcmp(temp, strC.data(), strC.count()) == 0);
    assert(temp[strC.count()] == '\0');

    //	reserve()
    auto internalBuf = str.data();
    auto cap = str.capacity();
    auto siz = str.count();

    str.reserve(cap);
    assert(internalBuf == str.data());
    assert(cap == str.capacity());
    assert(siz == str.count());

    str.reserve(cap + 1);
    assert(internalBuf != str.data());
    assert(cap + 1 == str.capacity());
    assert(siz == str.count());

    /////////////////
    //-	operator+=(Sträng sträng) som tolkas som konkatenering.
    //foo, bar, hej
    String str1("foo"), str2("bar"), str3("hej");
    ((str = "xyz") += str1) += (str3 += str1);
    assert(str3 == "hejfoo" && str == "xyzfoohejfoo" && str1 == "foo");

    //+= som får plats;
    str = "bar";
    str.reserve(10);
    str += "foo";
    assert(str == "barfoo");

    //+= som inte får plats;
    str.reserve(10);
    str = "";
    int i;
    for (i = 0; str.count() < str.capacity(); ++i)
        str.push_back('0' + i);
    str1 = "bar";
    str += str1;
    for (int k = 0; k < i; ++k)
        assert(str[k] == '0' + k);
    assert(str[i] == 'b');
    //+= Själv assignment	//Borde testa med att capacity tar slut!
    str = "foo";
    str += str;
    assert(str == "foofoo");

    //-	operator+
    str = "bar";
    assert(str + String("foo") == "barfoo");

    //move
    const char* strdata = str.data();
    String mStr(std::move(str));
    assert(strdata == mStr.data());
    assert(nullptr == str.data());

    str = std::move(mStr);
    assert(strdata == str.data());
    assert(nullptr == mStr.data());

    cout << "\nTestFörVälGodkänt klar\n";
}

#endif //VG


void TestIttInAlg() {

	static const int N = 26;
	String v;
	v.reserve(N);
	for (int i = 0; i < N; ++i) {
		v.push_back('a' + i);
	}
	v.begin();
	auto b = std::begin(v);
	auto e = std::end(v);

	std::random_shuffle(b, e);

	cout << v << endl;
	std::stable_sort(b, e);

	cout << v << endl;

}

void TestRevIttInAlg() {

	static const int N = 26;
	String v;
	v.reserve(N);
	for (int i = 0; i < N; ++i) {
		v.push_back('a' + i);
	}
	auto b = std::rbegin(v);
	auto e = std::rend(v);

	std::random_shuffle(b, e);

	cout << v << endl;
	std::stable_sort(b, e);

	cout << v << endl;

}

/*	*it, ++it, it++, (it+i), it[i], == och !=	*/
void TestIttPart() {
	String s1("foobar");
	for (auto i = s1.begin(); i != s1.end(); i++)
		cout << *i;
	cout << endl;
	//    s1 = "raboof";
	auto it = s1.begin();
	assert(*it == 'f');
	assert(*(it++) == 'f' && *it == 'o');
	++it;
	assert(*++it == 'b');
	cout << (++it < it);
	assert(*(it + 1) == 'a');
	assert(it[2] == 'r');
}

void TestIttPartR() {
	String s1("foobar");
	for (auto i = s1.rbegin(); i != s1.rend(); i++)
		cout << *i;
	cout << endl;
	s1 = "raboof";
	auto it = s1.rbegin();
	assert(*it == 'f');

	assert(*(it++) == 'f' && *it == 'o');
	++it;
	assert(*++it == 'b');
	assert(*(it + 1) == 'a');
	assert(it[2] == 'r');
}

#ifdef VG
void TestIttPartC() {
	String s1("foobar");
	for (auto i = s1.cbegin(); i != s1.cend(); i++)
		cout << *i;
	cout << endl;
	//    s1 = "raboof";
	auto it = s1.cbegin();
	assert(*it == 'f');
	assert(*(it++) == 'f' && *it == 'o');
	++it;
	assert(*++it == 'b');
	assert(*(it + 1) == 'a');
	assert(it[2] == 'r');
}

void TestIttPartCR() {
	String s1("foobar");
	for (auto i = s1.crbegin(); i != s1.crend(); i++)
		cout << *i;
	cout << endl;
	s1 = "raboof";
	auto it = s1.crbegin();
	assert(*it == 'f');
	assert(*(it++) == 'f' && *it == 'o');
	++it;
	assert(*++it == 'b');
	assert(*(it + 1) == 'a');
	assert(it[2] == 'r');
}
#endif VG

void TestFörGodkäntItt() {

	//-	typdefs för iterator, const_iterator,  reverse_iterator och const_revers_iterator
	String::iterator Str;
	String::reverse_iterator rStr;

	//-	funktionerna begin, end, cbegin, cend, rbegin, rend, crbegin och crend.


	TestIttPart();
	TestIttPartR();
#ifdef VG
	String::const_iterator cStr;
	String::const_reverse_iterator crStr;
	TestIttPartC();
	TestIttPartCR();
#endif VG

	//Iteratorerna ska kunna göra:
	//-	*it, ++it, it++, (it+i), it[i], == och !=



	//-	default constructor, copy constructor och tilldelning (=) 
	String s("foobar");
	Str = s.begin();
	rStr = s.rbegin();

#ifdef VG
	cStr = s.cbegin();
	crStr = s.crbegin();
#endif VG

	*Str = 'a';
	//	*(cStr+1)='b';	//Sak ge kompileringsfel!
	*(rStr + 2) = 'c';
	//	*(crStr+3)='d';	//Sak ge kompileringsfel!
	assert(s == "aoocar");

	cout << "\nTestFörGodkänt Itt klar\n";
#ifdef VG
	cout << "\nTestFörVälGodkänt Itt klar\n";
#endif VG

}
