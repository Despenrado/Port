#pragma once
#include "Ship.h"

using namespace std;

class ShipCrane
{
public:
    string state;
    mutex mtxState;
    Ship *myShip;
    atomic_int progress{0};
    mutex mtx;
    Container *currentContainer;
    vector<Container *> containerList;

    void unloadShip();
    void takeContainer();
    void giveContainer(Container &);
    void lifeCycle();
    void waitingForShip();
    void workSimulation(int);

    ShipCrane();
};
