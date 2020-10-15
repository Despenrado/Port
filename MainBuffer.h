#pragma once
#include "CarCrane.h"
#include "MainBufferCrane.h"

using namespace std;

class MainBuffer
{
private:
    /* data */
public:
    int id;
    bool isBusy;
    Car *car;
    CarCrane *carCrane;
    MainBufferCrane *bufferCrane;
    mutex mtxBusy;
    vector<int> car_order_load;
    vector<int> car_order_unload;

    MainBuffer();

    bool registerCarUnload(Car *);
    void unregisterCarUnload();
    bool unregisterCarLoad(Car *);
    bool registerCarLoad(Car *);
};
