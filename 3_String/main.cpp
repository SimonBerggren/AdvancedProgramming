#define _CRTDBG_MAP_ALLOC
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif

#include "String.h"
#include <stdlib.h>
#include <crtdbg.h>
#include <iostream>
#include "StringTest.h"

int main(int argC, char** argW)
{
	std::locale::global(std::locale("swedish"));
	TestFörGodkäntString();
#ifdef VG
	TestFörVälGodkäntString();
#endif VG

#ifdef ITT
	TestIttPart();
	TestIttPartR();
#ifdef VG
	TestIttPartC();
	TestIttPartCR();
#endif VG
	TestIttInAlg();
	TestRevIttInAlg();
#endif ITT

	system("pause");
	return EXIT_SUCCESS;
}