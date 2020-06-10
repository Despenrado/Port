#pragma once
#include "Dock.h"

using namespace std;

class Port
{
public:
    static atomic_bool *isRunning;
    static vector<Dock *> dockList;

    static bool registerShip(Ship *);
    static bool unregisterShip(Ship *);
    static vector<int> ship_order;
    static mutex mtx;

    static void genDockList(int);
};
