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

    Dock();

    void registerShipInDock(Ship *);
    void unregisterShipInDock();
};
