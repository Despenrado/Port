#pragma once
#include "Container.h"
#include "mutex"

using namespace std;

class DockBuffer
{
public:
    mutex mtx;
    vector<Container *> containerList;
    mutex work_mtx;

    DockBuffer();
};
