#include "passArrayToCPP.h"
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

int Worker::GetIntSum(uintptr_t arrayBuffer, int size)
{
    std::cout << "GetIntSum start work:" << std::endl;
    auto array = reinterpret_cast<int *>( arrayBuffer );
    int sum = 0;
    for( int i = 0; i < size; ++i )
    {
        sum = sum + array[i];
        std::cout << array[i] << std::endl;
    }
    std::cout << "sum: " << sum << std::endl;
    return sum;
}

double Worker::GetDoubleSum(uintptr_t arrayBuffer, int size)
{
    std::cout << "GetDoubleSum start work:" << std::endl;
    auto array = reinterpret_cast<double *>( arrayBuffer );
    double sum = 0;
    for( int i = 0; i < size; ++i )
    {
        sum = sum + array[i];
        std::cout << array[i] << std::endl;
    }
    std::cout << "sum: " << sum << std::endl;
    return sum;
}

void Worker::ShowBoolArray(uintptr_t arrayBuffer, int size)
{
    auto array = reinterpret_cast<bool *>( arrayBuffer );
    for( int i = 0; i < size; ++i )
    {
        std::cout << "bool: " << array[i] << std::endl;
    }
}

uintptr_t Worker::GetIntArray()
{
    int *values = new int[3];
    values[0] = 1;
    values[1] = 2;
    values[2] = 3;
    return uintptr_t( values );
}

uintptr_t Worker::GetFloatArray(int size)
{
    float values[size];
    for( int i = 0; i < size; ++i )
    {
        values[i] = i*2;
    }

    auto arrayPtr = uintptr_t( &values[0] );
    return arrayPtr;
}

Worker::Worker()
{
    std::cout << "generate Worker!" << std::endl;
}