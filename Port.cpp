#include "Port.h"
#include <algorithm>

void Port::genDockList(int n)
{
    for (int i = 0; i < n; i++)
    {
        Port::dockList.push_back(new Dock());
    }
}

void Port::genRailwayList(int n)
{
    for (int i = 0; i < n; i++)
    {
        Port::railwayList.push_back(new Railway());
    }
}

void Port::genMainBufferList(int n)
{
    for (int i = 0; i < n; i++)
    {
        Port::mainBufferList.push_back(new MainBuffer());
    }
}

OrderInfo *Port::getOrder()
{
    mtx.lock();
    for (int i = 0; i < order_queue.size(); i++)
    {
        for (int i2 = 0; i2 < mainBufferList.size(); i2++)
        {
            mainBufferList.at(i2)->mtxBusy.lock();
            for (int i3 = 0; i3 < mainBufferList.at(i2)->bufferCrane->buffer->containerList.size(); i3++)
            {
                mainBufferList.at(i2)->bufferCrane->mtx.lock();
                mainBufferList.at(i2)->bufferCrane->buffer->mtx.lock();
                if (order_queue.at(i)->containerId == mainBufferList.at(i2)->bufferCrane->buffer->containerList.at(i3)->id)
                {
                    OrderInfo *tmp = order_queue.at(i);
                    tmp->bufferId = i2;
                    order_queue.erase(order_queue.begin() + i);
                    mainBufferList.at(i2)->bufferCrane->buffer->mtx.unlock();
                    mainBufferList.at(i2)->bufferCrane->mtx.unlock();
                    mainBufferList.at(i2)->mtxBusy.unlock();
                    mtx.unlock();
                    return tmp;
                }
                mainBufferList.at(i2)->bufferCrane->buffer->mtx.unlock();
                mainBufferList.at(i2)->bufferCrane->mtx.unlock();
            }
            mainBufferList.at(i2)->mtxBusy.unlock();
        }
    }
    mtx.unlock();
    return new OrderInfo(-1, -1, -1);
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
            dockList.at(i)->registerShipInDock(newShip);
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
        if (dockList.at(i)->isBusy && dockList.at(i)->ship != NULL && dockList.at(i)->ship->id == oldShip->id)
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
    if (newCar->bufNum == -1)
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
        newCar->bufNum = max;
    }
    max = newCar->bufNum;
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
        if (mainBufferList.at(i)->isBusy && mainBufferList.at(i)->car != NULL && mainBufferList.at(i)->car->id == oldCar->id)
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
            mtx.unlock();
            return true;
        }
        mainBufferList.at(i)->carCrane->mtx.unlock();
        mainBufferList.at(i)->mtxBusy.unlock();
    }
    mtx.unlock();
    return false;
}

bool Port::registerMainBufferCarLoad(Car *newCar)
{
    mtx.lock();
    newCar->mtx.lock();
    int mainBufferNumber = newCar->orderInfo->bufferId;
    newCar->mtx.unlock();
    mtx.unlock();
    return mainBufferList.at(mainBufferNumber)->registerCarLoad(newCar);
}

bool Port::unregisterMainBufferCarLoad(Car *oldCar)
{
    mtx.lock();
    for (int i = 0; i < mainBufferList.size(); i++)
    {
        mainBufferList.at(i)->mtxBusy.lock();
        mainBufferList.at(i)->bufferCrane->mtx.lock();
        if (mainBufferList.at(i)->bufferCrane->myCar != NULL && mainBufferList.at(i)->bufferCrane->myCar->id == oldCar->id)
        {
            bool tmp = mainBufferList.at(i)->unregisterCarLoad(oldCar);
            mainBufferList.at(i)->bufferCrane->mtx.unlock();
            mainBufferList.at(i)->mtxBusy.unlock();
            mtx.unlock();
            return tmp;
        }
        mainBufferList.at(i)->bufferCrane->mtx.unlock();
        mainBufferList.at(i)->mtxBusy.unlock();
    }
    mtx.unlock();
    return false;
}

bool Port::registerRailWayCar(Car *newCar)
{
    mtx.lock();
    newCar->mtx.lock();
    newCar->currentContainer->mtx.lock();
    int railwayNumber = newCar->orderInfo->railwayId;
    newCar->currentContainer->mtx.unlock();
    newCar->mtx.unlock();
    mtx.unlock();
    return railwayList.at(railwayNumber)->registerCar(newCar);
}

bool Port::unregisterRailWayCar(Car *oldCar)
{
    mtx.lock();
    for (int i = 0; i < railwayList.size(); i++)
    {
        railwayList.at(i)->mtxBusy.lock();
        railwayList.at(i)->trainCrane->mtx.lock();
        if (railwayList.at(i)->isBusy && railwayList.at(i)->trainCrane->myCar != NULL && railwayList.at(i)->trainCrane->myCar->id == oldCar->id)
        {
            oldCar->mtx.lock();
            oldCar->state = "unparking";
            oldCar->mtx.unlock();
            railwayList.at(i)->trainCrane->mtx.unlock();
            railwayList.at(i)->mtxBusy.unlock();
            mtx.unlock();
            oldCar->workSimulation(10);
            mtx.lock();
            railwayList.at(i)->mtxBusy.lock();
            railwayList.at(i)->trainCrane->mtx.lock();
            railwayList.at(i)->unregisterCar(oldCar);
            railwayList.at(i)->isBusyCar = false;
            railwayList.at(i)->trainCrane->mtx.unlock();
            railwayList.at(i)->mtxBusy.unlock();
            mtx.unlock();
            return true;
        }
        railwayList.at(i)->trainCrane->mtx.unlock();
        railwayList.at(i)->mtxBusy.unlock();
    }
    mtx.unlock();
    return false;
}

bool Port::registerTrain(Train *newTrain)
{
    mtx.lock();
    newTrain->mtx.lock();
    if (train_queue.size() == 0 || newTrain->id != train_queue.at(0))
    {
        if (train_queue.size() == 0 || find(train_queue.begin(), train_queue.end(), newTrain->id) == train_queue.end())
        {
            train_queue.push_back(newTrain->id);
        }
        newTrain->mtx.unlock();
        mtx.unlock();
        return false;
    }
    newTrain->mtx.unlock();
    //mtx.unlock();
    for (int i = 0; i < railwayList.size(); i++)
    {
        railwayList.at(i)->mtxBusy.lock();
        railwayList.at(i)->trainCrane->mtx.lock();
        if (!railwayList.at(i)->isBusy)
        {
            newTrain->mtx.lock();
            newTrain->state = "arrival";
            newTrain->mtx.unlock();
            railwayList.at(i)->isBusy = true;
            train_queue.erase(train_queue.begin());
            railwayList.at(i)->trainCrane->mtx.unlock();
            railwayList.at(i)->mtxBusy.unlock();
            mtx.unlock();
            newTrain->workSimulation(10);
            mtx.lock();
            railwayList.at(i)->mtxBusy.lock();
            railwayList.at(i)->trainCrane->mtx.lock();
            newTrain->mtx.lock();
            for (int i2 = 0; i2 < newTrain->trainOrderList.size(); i2++)
            {
                order_queue.push_back(new OrderInfo(newTrain->trainOrderList.at(i2), -1, i));
            }
            //order_queue.insert(order_queue.end(), newTrain->trainOrderList.begin(), newTrain->trainOrderList.end());
            railwayList.at(i)->registerTrain(newTrain);
            newTrain->mtx.unlock();
            railwayList.at(i)->trainCrane->mtx.unlock();
            railwayList.at(i)->mtxBusy.unlock();
            mtx.unlock();
            return true;
        }
        railwayList.at(i)->trainCrane->mtx.unlock();
        railwayList.at(i)->mtxBusy.unlock();
    }
    mtx.unlock();
    return false;
}

bool Port::unregisterTrain(Train *oldTrain)
{
    mtx.lock();
    for (int i = 0; i < railwayList.size(); i++)
    {
        railwayList.at(i)->mtxBusy.lock();
        railwayList.at(i)->trainCrane->mtx.lock();
        oldTrain->mtx.lock();
        if (railwayList.at(i)->isBusy && railwayList.at(i)->trainCrane->myTrain != NULL && railwayList.at(i)->trainCrane->myTrain->id == oldTrain->id)
        {
            oldTrain->state = "departing";
            oldTrain->mtx.unlock();
            railwayList.at(i)->trainCrane->mtx.unlock();
            railwayList.at(i)->mtxBusy.unlock();
            mtx.unlock();
            oldTrain->workSimulation(10);
            mtx.lock();
            railwayList.at(i)->mtxBusy.lock();
            railwayList.at(i)->trainCrane->mtx.lock();
            railwayList.at(i)->unregisterTrain();
            railwayList.at(i)->isBusy = false;
            railwayList.at(i)->trainCrane->mtx.unlock();
            railwayList.at(i)->mtxBusy.unlock();
            mtx.unlock();
            return true;
        }
        oldTrain->mtx.unlock();
        railwayList.at(i)->trainCrane->mtx.unlock();
        railwayList.at(i)->mtxBusy.unlock();
    }
    mtx.unlock();
    return false;
}