#pragma once
#include "Orders.h"
#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

class Ship
{
private:
    /* data */
public:
    int id;
    string state;
    mutex mtxState;
    vector<Container *> containerList;
    int maxContainers;
    int waitingTime;
    atomic_int progress{0};
    mutex mtx;

    Ship();
    Ship(int id);

    void moor();
    void unMoor();
    void sail();
    void unloadContainers();
    void loadContainers();
    void takeContainer();
    Container *giveContainer();
    void lifeCycle();
    void workSimulation(int);
};
