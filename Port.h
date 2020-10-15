#pragma once
#include "Dock.h"
#include "MainBuffer.h"
#include "Railway.h"

using namespace std;

class Port
{
public:
    static atomic_bool *isRunning;
    static vector<Dock *> dockList;
    static vector<Railway *> railwayList;
    static vector<MainBuffer *> mainBufferList;

    static bool registerShip(Ship *);
    static bool unregisterShip(Ship *);
    static bool registerDockCar(Car *);
    static bool unregisterDockCar(Car *);

    static bool registerMainBufferCarLoad(Car *);
    static bool unregisterMainBufferCarLoad(Car *);
    static bool registerMainBufferCarUnload(Car *);
    static bool unregisterMainBufferCarUnload(Car *);
    static OrderInfo *getOrder();

    static bool registerRailWayCar(Car *);
    static bool unregisterRailWayCar(Car *);
    static bool registerTrain(Train *);
    static bool unregisterTrain(Train *);

    static vector<int> ship_order;
    static vector<int> train_queue;
    static vector<OrderInfo *> order_queue;
    static mutex mtx;

    static void genDockList(int);
    static void genMainBufferList(int);
    static void genRailwayList(int);
};
