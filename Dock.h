#pragma once
#include "Buffer.h"

using namespace std;

class Dock
{
private:
    /* data */
public:
    int id;
    bool isBusy;
    Ship *ship;
    Buffer *buffer;
    mutex mtxBusy;

    Dock();

    void registerShipInDock(Ship *);
    void unregisterShipInDock();
};
