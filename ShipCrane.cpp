#include "ShipCrane.h"

ShipCrane::ShipCrane()
{
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
    this->mtx.lock();
    if (myShip != NULL)
    {
        while (myShip->containerList.size() > 0)
        {
            this->mtx.unlock();
            takeContainer();
            this->mtx.lock();
        }
        this->mtx.unlock();
        return;
    }
    this->mtx.unlock();
    // put container to buffer
}

void ShipCrane::takeContainer()
{
    this->state = "taking container";
    if (myShip != NULL)
    {
        currentContainer = myShip->giveContainer();
        this->state = "taking container:" + to_string(currentContainer->id);
        containerList.push_back(currentContainer);
    }
    workSimulation(rand() % 10 + 15);
}

void ShipCrane::waitingForShip()
{
    this->state = "waiting for ship";
    this->mtx.lock();
    while (myShip == NULL)
    {
        this->mtx.unlock();
        workSimulation(rand() % 10 + 15);
        this->mtx.lock();
    }
    this->mtx.unlock();
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