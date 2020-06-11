#include "ShipCrane.h"

ShipCrane::ShipCrane()
{
    dockBuffer = new DockBuffer();
    this->myShip = NULL;
    this->currentContainer = NULL;
}

void ShipCrane::lifeCycle()
{
    while (true)
    {
        waitingForShip();
        unloadShip();
    }
}

void ShipCrane::unloadShip()
{
    //std::cout << "ShipCrane1" << std::endl;
    //this->mtx.lock();
    if (myShip != NULL)
    {
        //std::cout << "ShipCrane2" << std::endl;
        myShip->mtx.lock();
        while (myShip->containerList.size() > 0)
        {
            //std::cout << "ShipCrane3" << std::endl;
            myShip->mtx.unlock();
            this->mtx.unlock();
            //std::cout << "ShipCrane4" << std::endl;
            takeContainer();
            putContainer();
            this->mtx.lock();
            myShip->mtx.lock();
        }
        myShip->mtx.unlock();
        this->mtx.unlock();
        return;
    }
    this->mtx.unlock();
}

void ShipCrane::takeContainer()
{
    this->mtx.lock();
    //std::cout << "ShipCrane5" << std::endl;
    this->state = "taking container";
    if (myShip != NULL)
    {
        currentContainer = myShip->giveContainer();
        this->state = "taking container:" + to_string(currentContainer->id);
    }
    this->mtx.unlock();
    workSimulation(rand() % 10 + 15);
}

void ShipCrane::putContainer()
{
    this->mtx.lock();
    this->state = "putting container:" + to_string(currentContainer->id);
    this->mtx.unlock();
    dockBuffer->work_mtx.lock();
    dockBuffer->mtx.lock();
    this->mtx.lock();
    dockBuffer->containerList.push_back(currentContainer);
    this->mtx.unlock();
    dockBuffer->mtx.unlock();
    workSimulation(rand() % 10 + 15);
    dockBuffer->work_mtx.unlock();
}

void ShipCrane::waitingForShip()
{
    //std::cout << "waitingForShip1" << std::endl;
    this->mtx.lock();
    this->state = "waiting for ship";
    //std::cout << "waitingForShip2" << std::endl;
    while (myShip == NULL)
    {
        //std::cout << "waitingForShip3" << std::endl;
        this->mtx.unlock();
        //std::cout << "waitingForShip4" << std::endl;
        workSimulation(rand() % 10 + 15);
        //std::cout << "waitingForShip5" << std::endl;
        this->mtx.lock();
        //std::cout << "waitingForShip6" << std::endl;
    }
    //this->mtx.unlock();
    //std::cout << "waitingForShip7" << std::endl;
}

void ShipCrane::workSimulation(int times)
{
    for (int i = 0; i < times; i++)
    {
        this_thread::sleep_for(chrono::milliseconds(100));
        progress = float(i * 100) / times;
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    progress = 0;
}