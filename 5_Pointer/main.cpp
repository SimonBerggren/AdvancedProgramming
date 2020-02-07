#ifdef _DEBUG
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif

#include <cassert>
#include <iostream>
#include "SharedPointer.h"
#include "WeakPointer.h"

using std::cout;
using std::cin;

void TestG();
void TestVG();

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	std::locale::global(std::locale("swedish"));

	TestG();
	TestVG();

	cin.get();
}

struct C {
	float value;
	C(float value) :value(value) {};
};

void TestG() {
	//-	Konstruktor som tar:	
	//	o	inget	G
	//	o	En SharedPtr	G
	//	o	En pekare	G

	SharedPointer<C> sp11;
	assert(!sp11);
	SharedPointer<C> p15(nullptr);
	assert(!p15);
	SharedPointer<C> sp12(new C(12));
	assert(sp12);
	SharedPointer<C> sp13(sp11);
	assert(!sp13);

	assert(sp12.unique());
	SharedPointer<C> sp14(sp12);
	assert(sp14);
	assert(!sp12.unique());

	//-	Destruktor	G
	//It will test itself
	//-	Tilldelning från en	
	//	o	En SharedPtr	G
	sp14 = sp12;
	assert(sp14);

	sp14 = sp14;
	assert(sp14);

	//-	Jämförelse med (== och <)
	SharedPointer<C> sp31(new C(31));
	//	o	En SharedPtr	G
	assert(sp11 == nullptr);
	assert(sp11 < sp12);
	assert(!(sp12 < sp11));
	assert(sp14 == sp12);
	assert(!(sp14 == sp31));
	assert((sp14 < sp31) || (sp31 < sp14));

	//get, * och ->

	SharedPointer<C> sp41(new C(41));
	SharedPointer<C> sp42(new C(42));
	assert((sp41->value) == (sp41.get()->value));
	assert((sp41->value) != (sp42.get()->value));
	assert(&(*sp41) == (sp41.get()));

	//move
	SharedPointer<C> sp51(std::move(sp41));
	assert(sp51->value == 41);
	assert(!sp41);

	sp51.reset();
	assert(!sp51);
}

void TestVG() {
	//Weak pointer skall ha det som det står VG på nedan
	//-	Konstruktor som tar:		
	//	o	inget	G	VG
	//	o	En SharedPtr	G	VG
	//	o	En WeakPtr	VG	VG

	WeakPointer<C> wp11;
	assert(wp11.expired());
	SharedPointer<C> sp12(new C(12));
	WeakPointer<C> wp13(wp11);
	assert(wp13.expired());
	WeakPointer<C> wp14(sp12);
	assert(!wp14.expired());

	SharedPointer<C> sp17(wp14);
	assert(sp17);

	//-	Destruktor	G	VG
	//	It will test itself
	//-	Tilldelning från en		
	//	o	En SharedPtr	G	VG
	//	o	En WeakPtr			VG
	WeakPointer<C> wp15;
	wp14 = wp11;
	assert(wp14.expired());

	SharedPointer<C> sp33(new C(33));
	wp14 = sp33;
	assert(!wp14.expired());
	wp14 = wp14;
	assert(!wp14.expired());

	sp33.reset();
	assert(!sp33);
	assert(wp14.expired());

	//Shared(weak)
	try {
		SharedPointer<C> slask(wp14);
	}
	catch (const char* const except) {
		assert(except == "std::bad_weak_ptr");
	}

	//-	funktioner:		
	//	o	lock()		VG
	auto sp51 = wp11.lock();
	assert(!sp51);

	SharedPointer<C>  sp55(new C(55));
	wp14 = sp55;
	sp51 = wp14.lock();
	assert(sp51);
	////	o	expired()		VG	Redan testat

	//move
	SharedPointer<C> sp61(std::move(sp51));
	assert(sp61->value == 55);
	assert(!sp51);

	sp51 = std::move(sp61);
	sp51 = std::move(sp51);
	auto v = sp51->value;
	assert(sp51->value == 55);
}