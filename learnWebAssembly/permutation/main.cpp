#include <iostream>
#include<cstdio>
#include<algorithm>

#include "permutation.h"
using namespace std; 

int main(){
	Permutation obj;
	obj.InputArray( "1 2 3 4 5" );
	auto str = obj.GetPermutations();
	cout << str;
	return 0;
}
