#pragma once
#include "Car.h"
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
    Car *myCar;
    //vector<Container *> containerList;
    DockBuffer *dockBuffer;

    void takeContainer();
    void putToCar();
    void lifeCycle();
    void workSimulation(int);
    void registerCarInDock(Car *);
    void unregisterCarInDock();

    BufferCrane(DockBuffer *);
};
