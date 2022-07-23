#include <iostream>
#include "ServiceX.h"

int main()
{
	std::cout << "This is main()" << '\n';

	ServiceX sX("test", "test");
	sX.preInit();

	return 0;
}