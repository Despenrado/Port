#pragma once
#include "Orders.h"
#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

class Car
{
private:
    /* data */
public:
    int id;
    string state;
    mutex mtxState;
    Container *currentContainer;
    atomic_int progress{0};
    mutex mtx;

    Car();
    Car(int id);

    void loadDock();
    void loadTrain();
    void unloadDock();
    void unloadTrain();
    void rideFree();
    void rideFull();
    Container *giveContainer();
    void takeContainer(Container *);
    void lifeCycle();
    void workSimulation(int);
};