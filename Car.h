#pragma once
#include "OrderInfo.h"
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
    int bufNum;
    OrderInfo *orderInfo;

    Car();
    Car(int id);

    void loadFromDock();
    void loadFromMainBuffer();
    void unloadToMainBuffer();
    void unloadToTrain();
    void rideFree();
    void rideFull();
    Container *giveContainer();
    void takeContainer(Container *);
    void lifeCycle();
    void workSimulation(int);
};