#include "Dock.h"
#include <algorithm>

Dock::Dock()
{
    this->shipCrane = new ShipCrane();
    this->bufferCrane = new BufferCrane(shipCrane->buffer);
}

void Dock::registerShipInDock(Ship *ship)
{
    this->shipCrane->myShip = ship;
    this->ship = ship;
}

void Dock::unregisterShipInDock()
{
    this->shipCrane->myShip = NULL;
    this->ship = NULL;
}

bool Dock::registerCar(Car *newCar)
{

    this->mtxBusy.lock();
    newCar->mtx.lock();
    if (car_order.size() == 0 || newCar->id != car_order.at(0))
    {
        if (car_order.size() == 0 || find(car_order.begin(), car_order.end(), newCar->id) == car_order.end())
        {
            car_order.push_back(newCar->id);
        }
        newCar->mtx.unlock();
        this->mtxBusy.unlock();
        return false;
    }
    newCar->mtx.unlock();
    this->bufferCrane->mtx.lock();
    if (this->bufferCrane->myCar == NULL)
    {
        newCar->mtx.lock();
        newCar->state = "parking";
        newCar->dockNum = -1;
        newCar->mtx.unlock();
        this->bufferCrane->registerCar(newCar);
        this->car_order.erase(this->car_order.begin());
        this->bufferCrane->mtx.unlock();
        this->mtxBusy.unlock();
        newCar->workSimulation(10);
        return true;
    }
    this->bufferCrane->mtx.unlock();
    this->mtxBusy.unlock();
    return false;
}

bool Dock::unregisterCar(Car *oldCar)
{
    this->bufferCrane->unregisterCar();
    return true;
}