#include <iostream>

#include "TutorialIf.h"



int main()
{

	std::cout << "Test" << '\n';

	return 0;
}



/*
NOTES:


Compile:
/home/marin/dev/BlackCat/sw/Mod-SandBox0
$ make unitTestTut

Run:
/home/marin/dev/BlackCat/bins/Mod-SandBox0_UT
$ ./Mod-SandBox0_UT_Srv_Tutorial_UDEMY_Cpp_Algos_0


(TOOLS)
GDB:


Valgrind:
// Make sure that compile with flag -g (debugging info)

/home/marin/dev/BlackCat/bins/Mod-SandBox0_UT
$ valgrind --leak-check=full ./Mod-SandBox0_UT_Srv_Tutorial_UDEMY_Cpp_Algos_0


*/