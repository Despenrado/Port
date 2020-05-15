#include "Buffer.h"

Buffer::Buffer()
{
}

void Buffer::lifeCycle()
{
    while (true)
    {
        waitingForShip();
        unloadShip();
    }
}

void Buffer::unloadShip()
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
}

void Buffer::takeContainer()
{
    this->state = "waiting for ship";
    if (myShip != NULL)
    {
        containerList.push_back(myShip->giveContainer());
    }
    workSimulation(rand() % 10 + 15);
}

void Buffer::waitingForShip()
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

void Buffer::workSimulation(int times)
{
    for (int i = 0; i < times; i++)
    {
        this_thread::sleep_for(chrono::milliseconds(1));
        progress = float(i * 100) / times;
        this_thread::sleep_for(chrono::milliseconds(1));
    }
    progress = 0;
}