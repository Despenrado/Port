#pragma once
#include "Container.h"
#include <atomic>
#include <iostream>
#include <vector>

using namespace std;

class Train
{

public:
    int id;
    string state;
    mutex mtxState;
    vector<Container *> containerList;
    vector<int> trainOrderList;
    int maxContainers;
    atomic_int progress{0};
    mutex mtx;

    Train();
    Train(int id);

    void takeOrders();
    void stopAtStation();
    void leftFromStation();
    void ride();
    void unloadContainers();
    void loadContainers();
    void takeContainer(Container *);
    Container *giveContainer();
    void lifeCycle();
    void workSimulation(int);
};
