#pragma once
#include "DockBuffer.h"
#include <atomic>

using namespace std;

class BufferCrane
{
public:
    string state;
    mutex mtxState;
    atomic_int progress{0};
    mutex mtx;
    Container *currentContainer;
    //vector<Container *> containerList;
    DockBuffer *dockBuffer;

    void takeContainer();
    void putToCar();
    void lifeCycle();
    void workSimulation(int);

    BufferCrane(DockBuffer *);
};
