#include "MainBuffer.h"
#include <algorithm>

MainBuffer::MainBuffer()
{
    this->carCrane = new CarCrane();
    this->bufferCrane = new BufferCrane(carCrane->buffer);
    this->car = NULL;
}

bool MainBuffer::registerCarUnload(Car *newCar)
{
    this->mtxBusy.lock();
    newCar->mtx.lock();
    if (car_order_unload.size() == 0 || newCar->id != car_order_unload.at(0))
    {
        if (car_order_unload.size() == 0 || find(car_order_unload.begin(), car_order_unload.end(), newCar->id) == car_order_unload.end())
        {
            car_order_unload.push_back(newCar->id);
        }
        newCar->mtx.unlock();
        this->mtxBusy.unlock();
        return false;
    }
    newCar->mtx.unlock();
    this->carCrane->mtx.lock();
    if (!isBusy)
    {
        newCar->mtx.lock();
        newCar->state = "parking";
        newCar->mtx.unlock();
        this->isBusy = true;
        car_order_unload.erase(car_order_unload.begin());
        this->carCrane->mtx.unlock();
        this->mtxBusy.unlock();
        newCar->workSimulation(10);
        this->mtxBusy.lock();
        this->carCrane->mtx.lock();
        this->car = newCar;
        this->carCrane->myCar = newCar;
        this->carCrane->mtx.unlock();
        this->mtxBusy.unlock();
        return true;
    }
    this->carCrane->mtx.unlock();
    this->mtxBusy.unlock();
    return false;

    this->carCrane->myCar = car;
    this->car = car;
}

void MainBuffer::unregisterCarUnload()
{
    this->carCrane->myCar = NULL;
    this->car = NULL;
}

bool MainBuffer::registerCarLoad(Car *newCar)
{
    this->mtxBusy.lock();
    newCar->mtx.lock();
    if (car_order_load.size() == 0 || newCar->id != car_order_load.at(0))
    {
        if (car_order_load.size() == 0 || find(car_order_load.begin(), car_order_load.end(), newCar->id) == car_order_load.end())
        {
            car_order_load.push_back(newCar->id);
        }
        newCar->mtx.unlock();
        this->mtxBusy.unlock();
        return false;
    }
    newCar->mtx.unlock();
    this->bufferCrane->mtx.lock();
    if (this->bufferCrane->myCar == NULL)
    {
        this->bufferCrane->mtx.unlock();
        newCar->mtx.lock();
        newCar->state = "parking";
        newCar->mtx.unlock();
        this->bufferCrane->registerCar(newCar);
        this->car_order_load.erase(this->car_order_load.begin());
        this->mtxBusy.unlock();
        newCar->workSimulation(10);
        return true;
    }
    this->bufferCrane->mtx.unlock();
    this->mtxBusy.unlock();
    return false;
}

bool MainBuffer::unregisterCarLoad(Car *oldCar)
{
    oldCar->workSimulation(10);
    this->bufferCrane->unregisterCar();
    return true;
}