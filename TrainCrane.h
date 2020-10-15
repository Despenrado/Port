#pragma once
#include "Car.h"
#include "Train.h"

using namespace std;

class TrainCrane
{
public:
    string state;
    mutex mtxState;
    Train *myTrain;
    Car *myCar;
    atomic_int progress{0};
    mutex mtx;
    Container *currentContainer;

    //    void unloadCar();
    void takeContainer();
    void lifeCycle();
    void waitingForCar();
    void waitingForTrain();
    void workSimulation(int);
    void putContainerToTrain();

    TrainCrane();
};
