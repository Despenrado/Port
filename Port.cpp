#include "Port.h"

bool Port::registerShip(Ship *newShip)
{
    for (int i = 0; i < dockList.size(); i++)
    {
        dockList.at(i)->mtxBusy.lock();
        if (!dockList.at(i)->isBusy)
        {
            newShip->mtx.lock();
            newShip->state = "mooring";
            newShip->mtx.unlock();
            dockList.at(i)->registerShipInDock(newShip);
            dockList.at(i)->isBusy = true;
            dockList.at(i)->mtxBusy.unlock();
            newShip->workSimulation(10);
            return true;
        }
        dockList.at(i)->mtxBusy.unlock();
    }
    return false;
}

bool Port::unregisterShip(Ship *oldShip)
{
    for (int i = 0; i < dockList.size(); i++)
    {
        dockList.at(i)->mtxBusy.lock();
        if (dockList.at(i)->isBusy && dockList.at(i)->ship->id == oldShip->id)
        {
            oldShip->mtx.lock();
            oldShip->state = "unmooring";
            oldShip->mtx.unlock();
            oldShip->workSimulation(10);
            dockList.at(i)->unregisterShipInDock();
            dockList.at(i)->isBusy = false;
            dockList.at(i)->mtxBusy.unlock();
            return true;
        }
        dockList.at(i)->mtxBusy.unlock();
    }
    return false;
}

void Port::genDockList(int n)
{
    for (int i = 0; i < n; i++)
    {
        Port::dockList.push_back(new Dock());
    }
}