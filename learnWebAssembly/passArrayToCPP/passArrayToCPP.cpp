#include "passArrayToCPP.h"
#include <iostream>

int Worker::GetIntGum(uintptr_t arrayBuffer, int size)
{
    std::cout << "hello" << std::endl;
    auto array = reinterpret_cast<int *>( arrayBuffer );
    int sum = 0;
    for( int i = 0; i < size; ++i )
    {
        sum = sum + array[i];
        std::cout << array[i] << std::endl;
    }
    std::cout << sum << std::endl;
    return sum;
}