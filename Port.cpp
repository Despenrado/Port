#include "Port.h"
#include <algorithm>

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
            dockList.at(i)->registerShipInDock(newShip);
            dockList.at(i)->isBusy = true;
            std::cout << "mooring1" << std::endl;
            mtx.lock();
            std::cout << "mooring2" << std::endl;
            ship_order.erase(ship_order.begin());
            mtx.unlock();
            std::cout << "mooring3" << std::endl;
            dockList.at(i)->shipCrane->mtx.unlock();
            dockList.at(i)->mtxBusy.unlock();
            newShip->workSimulation(10);
            return true;
        }
        dockList.at(i)->shipCrane->mtx.unlock();
        dockList.at(i)->mtxBusy.unlock();
    }
    return false;
}

bool Port::unregisterShip(Ship *oldShip)
{
    for (int i = 0; i < dockList.size(); i++)
    {
        dockList.at(i)->mtxBusy.lock();
        dockList.at(i)->shipCrane->mtx.lock();
        if (dockList.at(i)->isBusy && dockList.at(i)->ship->id == oldShip->id)
        {
            oldShip->mtx.lock();
            oldShip->state = "unmooring";
            oldShip->mtx.unlock();
            std::cout << "unmooring" << std::endl;
            dockList.at(i)->shipCrane->mtx.unlock();
            dockList.at(i)->mtxBusy.unlock();
            oldShip->workSimulation(10);
            dockList.at(i)->mtxBusy.lock();
            dockList.at(i)->shipCrane->mtx.lock();
            dockList.at(i)->unregisterShipInDock();
            std::cout << "unmooring2" << std::endl;
            dockList.at(i)->isBusy = false;
            std::cout << "unmooring3" << std::endl;
            dockList.at(i)->shipCrane->mtx.unlock();
            dockList.at(i)->mtxBusy.unlock();
            std::cout << "unmooring4" << std::endl;
            return true;
        }
        dockList.at(i)->shipCrane->mtx.unlock();
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