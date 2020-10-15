#include "Railway.h"
#include <algorithm>

Railway::Railway()
{
    this->isBusy = false;
    this->isBusyCar = false;
    this->trainCrane = new TrainCrane();
}

void Railway::registerTrain(Train *train)
{
    this->trainCrane->myTrain = train;
}

void Railway::unregisterTrain()
{
    this->trainCrane->myTrain = NULL;
}

bool Railway::registerCar(Car *newCar)
{
    this->mtxBusy.lock();
    newCar->mtx.lock();
    if (car_queue.size() == 0 || newCar->id != car_queue.at(0))
    {
        //newCar->state = to_string(1);
        if (car_queue.size() == 0 || find(car_queue.begin(), car_queue.end(), newCar->id) == car_queue.end())
        {
            car_queue.push_back(newCar->id);
        }
        newCar->mtx.unlock();
        this->mtxBusy.unlock();
        return false;
    }
    //newCar->state = to_string(2);
    newCar->mtx.unlock();
    this->trainCrane->mtx.lock();
    //newCar->state = to_string(3);
    if (!isBusyCar)
    {
        //newCar->state = to_string(4);
        newCar->mtx.lock();
        newCar->state = "parking";
        newCar->mtx.unlock();
        this->isBusyCar = true;
        car_queue.erase(car_queue.begin());
        this->trainCrane->mtx.unlock();
        this->mtxBusy.unlock();
        newCar->workSimulation(10);
        this->mtxBusy.lock();
        this->trainCrane->mtx.lock();
        this->trainCrane->myCar = newCar;
        this->trainCrane->mtx.unlock();
        this->mtxBusy.unlock();
        return true;
    }
    this->trainCrane->mtx.unlock();
    this->mtxBusy.unlock();
    return false;
}

void Railway::unregisterCar(Car *oldCar)
{
    this->trainCrane->myCar = NULL;
}
