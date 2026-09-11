#include "worker.h"

int main()
{
    auto workerObj = new Worker();
    workerObj->Init();
    workerObj->Start();

    return 0;
}
