#pragma once

#include <string>

class Worker
{
public:
    Worker();

    int GetIntSum(uintptr_t arrayBuffer, int size);
    double GetDoubleSum(uintptr_t arrayBuffer, int size);
    void ShowBoolArray(uintptr_t arrayBuffer, int size);
    
    uintptr_t GetIntArray();
    uintptr_t GetFloatArray(int size);
};