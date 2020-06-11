#pragma once
#include "Buffer.h"
#include "Car.h"

using namespace std;

class CarCrane
{
public:
    string state;
    mutex mtxState;
    Car *myCar;
    atomic_int progress{0};
    mutex mtx;
    Container *currentContainer;
    //vector<Container *> containerList;
    Buffer *buffer;

    void unloadCar();
    void takeContainer();
    void giveContainer(Container &);
    void lifeCycle();
    void waitingForCar();
    void workSimulation(int);
    void putContainer();

    CarCrane();
};