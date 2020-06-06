#pragma once
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
    mutex mtxBusy;

    Dock();

    void registerShipInDock(Ship *);
    void unregisterShipInDock();
};
