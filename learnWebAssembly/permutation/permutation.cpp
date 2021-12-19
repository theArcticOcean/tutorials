#include "permutation.h"

#include <algorithm>
#include <cstring>
#include <cstdio>
#include <iostream>

void Permutation::InputArray(std::string str)
{
	memset( strBuffer, 0, LEN );
	elements.clear();
	auto pos = str.find( " " );
	while( pos != std::string::npos )
	{
		std::string headStr = str.substr( 0, pos );
		if( headStr.size() > 0 )
		{
			elements.push_back( atoi( headStr.c_str() ) );
		}
		str = str.substr( pos+1, str.size() - headStr.size() );
		pos = str.find( " " );
	}

	if( str.size() > 0 )
	{
		elements.push_back( atoi( str.c_str() ) );
	}

	for( auto value: elements )
	{
		printf( "%d\t", value );
	}
	printf( "\n" );
}

void Permutation::PrintCase()
{
	for( auto value: elements )
	{
		sprintf( strBuffer + strlen(strBuffer), "%d\t", value );
	}
	sprintf( strBuffer + strlen(strBuffer), "\n" );
}

std::string Permutation::GetPermutations()
{
	int sum=0;
	do{
		PrintCase();
		sum++;
	}while(next_permutation(elements.begin(),elements.end()));
	sprintf( strBuffer + strlen(strBuffer), "sum = %d\n", sum );

	return std::string( strBuffer );
}