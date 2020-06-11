#pragma once
#include "Container.h"
#include "mutex"

using namespace std;

class Buffer
{
public:
    mutex mtx;
    vector<Container *> containerList;
    mutex work_mtx;

    Buffer();
};
