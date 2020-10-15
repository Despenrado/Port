#pragma once
#include "TrainCrane.h"

using namespace std;

class Railway
{
private:
    /* data */
public:
    int id;
    bool isBusy;
    bool isBusyCar;
    TrainCrane *trainCrane;
    mutex mtxBusy;
    vector<int> car_queue;

    Railway();

    void registerTrain(Train *);
    void unregisterTrain();
    bool registerCar(Car *);
    void unregisterCar(Car *);
};
