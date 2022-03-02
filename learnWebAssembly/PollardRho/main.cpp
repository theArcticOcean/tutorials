#include "resolver.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
	Resolver worker;
	int x = std::stoi( argv[1] );
	std::cout << worker.GetResolvedResult( x ) << std::endl;
	return 0;
}
