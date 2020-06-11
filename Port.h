#pragma once
#include "Dock.h"

using namespace std;

class Port
{
public:
    static atomic_bool *isRunning;
    static vector<Dock *> dockList;

    static bool registerShip(Ship *);
    static bool registerDockCar(Car *);
    static bool registerMainBufferCar(Car *);
    static bool registerRailWayCar(Car *);
    static bool unregisterDockCar(Car *);
    static bool unregisterMainBufferCar(Car *);
    static bool unregisterRailWayCar(Car *);
    static bool unregisterShip(Ship *);
    static vector<int> ship_order;
    static mutex mtx;

    static void genDockList(int);
};
