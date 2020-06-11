#pragma once
#include "BufferCrane.h"
#include "ShipCrane.h"

using namespace std;

class Dock
{
private:
    /* data */
public:
    int id;
    bool isBusy;
    Ship *ship;
    ShipCrane *shipCrane;
    BufferCrane *bufferCrane;
    mutex mtxBusy;
    vector<int> car_order;

    Dock();

    void registerShipInDock(Ship *);
    void unregisterShipInDock();
    bool unregisterCarInDock(Car *);
    bool registerCarInDock(Car *);
};
