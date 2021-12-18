#include "resolver.h"
#include <iostream>

int main()
{
	Resolver worker;
	std::cout << worker.GetResolvedResult( 19950212 ) << std::endl;
	return 0;
}
