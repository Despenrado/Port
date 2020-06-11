#include "Port.h"
#include <algorithm>

void Port::genDockList(int n)
{
    for (int i = 0; i < n; i++)
    {
        Port::dockList.push_back(new Dock());
    }
}

void Port::genMainBufferList(int n)
{
    for (int i = 0; i < n; i++)
    {
        Port::mainBufferList.push_back(new MainBuffer());
    }
}

bool Port::registerShip(Ship *newShip)
{

    mtx.lock();
    newShip->mtx.lock();
    if (ship_order.size() == 0 || newShip->id != ship_order.at(0))
    {
        if (ship_order.size() == 0 || find(ship_order.begin(), ship_order.end(), newShip->id) == ship_order.end())
        {
            ship_order.push_back(newShip->id);
        }
        newShip->mtx.unlock();
        mtx.unlock();
        return false;
    }
    newShip->mtx.unlock();
    mtx.unlock();
    for (int i = 0; i < dockList.size(); i++)
    {
        dockList.at(i)->mtxBusy.lock();
        dockList.at(i)->shipCrane->mtx.lock();
        if (!dockList.at(i)->isBusy)
        {
            newShip->mtx.lock();
            newShip->state = "mooring";
            newShip->mtx.unlock();
            dockList.at(i)->isBusy = true;
            //std::cout << "mooring1" << std::endl;
            mtx.lock();
            //std::cout << "mooring2" << std::endl;
            ship_order.erase(ship_order.begin());
            mtx.unlock();
            //std::cout << "mooring3" << std::endl;
            dockList.at(i)->shipCrane->mtx.unlock();
            dockList.at(i)->mtxBusy.unlock();
            newShip->workSimulation(10);
            dockList.at(i)->mtxBusy.lock();
            dockList.at(i)->shipCrane->mtx.lock();
            dockList.at(i)->registerShipInDock(newShip); //
            dockList.at(i)->shipCrane->mtx.unlock();
            dockList.at(i)->mtxBusy.unlock();
            return true;
        }
        dockList.at(i)->shipCrane->mtx.unlock();
        dockList.at(i)->mtxBusy.unlock();
    }
    return false;
}

bool Port::unregisterShip(Ship *oldShip)
{
    mtx.lock();
    for (int i = 0; i < dockList.size(); i++)
    {
        dockList.at(i)->mtxBusy.lock();
        dockList.at(i)->shipCrane->mtx.lock();
        oldShip->mtx.lock();
        if (dockList.at(i)->isBusy && dockList.at(i)->ship->id == oldShip->id)
        {
            oldShip->state = "unmooring";
            oldShip->mtx.unlock();
            //std::cout << "unmooring" << std::endl;
            dockList.at(i)->shipCrane->mtx.unlock();
            dockList.at(i)->mtxBusy.unlock();
            mtx.unlock();
            oldShip->workSimulation(10);
            mtx.lock();
            dockList.at(i)->mtxBusy.lock();
            dockList.at(i)->shipCrane->mtx.lock();
            dockList.at(i)->unregisterShipInDock();
            //std::cout << "unmooring2" << std::endl;
            dockList.at(i)->isBusy = false;
            //std::cout << "unmooring3" << std::endl;
            dockList.at(i)->shipCrane->mtx.unlock();
            dockList.at(i)->mtxBusy.unlock();
            //std::cout << "unmooring4" << std::endl;
            mtx.unlock();
            return true;
        }
        oldShip->mtx.unlock();
        dockList.at(i)->shipCrane->mtx.unlock();
        dockList.at(i)->mtxBusy.unlock();
    }
    mtx.unlock();
    return false;
}

bool Port::registerDockCar(Car *newCar)
{
    mtx.lock();
    newCar->mtx.lock();
    int max = 0;
    if (newCar->dockNum == -1)
    {
        for (int i = 0; i < dockList.size(); i++)
        {
            dockList.at(i)->mtxBusy.lock();
            dockList.at(i)->shipCrane->mtx.lock();
            dockList.at(i)->shipCrane->buffer->mtx.lock();
            if (dockList.at(max)->shipCrane->buffer->containerList.size() < dockList.at(i)->shipCrane->buffer->containerList.size())
            {
                max = i;
            }
            dockList.at(i)->shipCrane->buffer->mtx.unlock();
            dockList.at(i)->shipCrane->mtx.unlock();
            dockList.at(i)->mtxBusy.unlock();
        }
        newCar->dockNum = max;
    }
    max = newCar->dockNum;
    newCar->mtx.unlock();
    mtx.unlock();
    return dockList.at(max)->registerCar(newCar);
}

bool Port::unregisterDockCar(Car *oldCar)
{
    mtx.lock();
    for (int i = 0; i < dockList.size(); i++)
    {
        dockList.at(i)->mtxBusy.lock();
        dockList.at(i)->bufferCrane->mtx.lock();
        if (dockList.at(i)->bufferCrane->myCar != NULL && dockList.at(i)->bufferCrane->myCar->id == oldCar->id)
        {
            bool tmp = dockList.at(i)->unregisterCar(oldCar);
            dockList.at(i)->bufferCrane->mtx.unlock();
            dockList.at(i)->mtxBusy.unlock();
            mtx.unlock();
            return tmp;
        }
        dockList.at(i)->bufferCrane->mtx.unlock();
        dockList.at(i)->mtxBusy.unlock();
    }
    mtx.unlock();
    return false;
}

bool Port::registerMainBufferCarLoad(Car *newCar)
{
    return false;
}
bool Port::unregisterMainBufferCarLoad(Car *oldCar)
{
    return false;
}
bool Port::registerMainBufferCarUnload(Car *newCar)
{
    mtx.lock();
    newCar->mtx.lock();
    newCar->currentContainer->mtx.lock();
    int mainBufferNumber = newCar->currentContainer->id % mainBufferList.size();
    newCar->currentContainer->mtx.unlock();
    newCar->mtx.unlock();
    mtx.unlock();
    return mainBufferList.at(mainBufferNumber)->registerCarUnload(newCar);
}
bool Port::unregisterMainBufferCarUnload(Car *oldCar)
{
    mtx.lock();
    for (int i = 0; i < mainBufferList.size(); i++)
    {
        mainBufferList.at(i)->mtxBusy.lock();
        mainBufferList.at(i)->carCrane->mtx.lock();
        if (mainBufferList.at(i)->isBusy && mainBufferList.at(i)->car->id == oldCar->id)
        {
            oldCar->mtx.lock();
            oldCar->state = "unparking";
            oldCar->mtx.unlock();
            mainBufferList.at(i)->carCrane->mtx.unlock();
            mainBufferList.at(i)->mtxBusy.unlock();
            mtx.unlock();
            oldCar->workSimulation(10);
            mtx.lock();
            mainBufferList.at(i)->mtxBusy.lock();
            mainBufferList.at(i)->carCrane->mtx.lock();
            mainBufferList.at(i)->unregisterCarUnload();
            mainBufferList.at(i)->isBusy = false;
            mainBufferList.at(i)->carCrane->mtx.unlock();
            mainBufferList.at(i)->mtxBusy.unlock();
            mtx.lock();
            return true;
        }
        mainBufferList.at(i)->carCrane->mtx.unlock();
        mainBufferList.at(i)->mtxBusy.unlock();
    }
    mtx.unlock();
    return false;
}
