#include "resolver.h"
#include <iostream>

int main()
{
	Resolver worker;
	std::cout << worker.GetResolvedResult( 100 ) << std::endl;
	return 0;
}
