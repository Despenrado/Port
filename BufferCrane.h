#pragma once
#include "Buffer.h"
#include "Car.h"
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
    Buffer *buffer;

    void takeContainer();
    void putToCar();
    void lifeCycle();
    void workSimulation(int);
    void registerCar(Car *);
    void unregisterCar();

    BufferCrane(Buffer *);
};
