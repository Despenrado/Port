#include "BufferCrane.h"
#include "Orders.h"
#include "Port.h"
#include <sstream>

BufferCrane::BufferCrane(Buffer *tmp)
{
    this->buffer = tmp;
    this->myCar = NULL;
    this->currentContainer = NULL;
}

void BufferCrane::lifeCycle()
{
    while (Port::isRunning)
    {
        takeContainer();
        if (!Port::isRunning)
        {
            this->mtx.unlock();
            return;
        }
        putToCar();
    }
}

void BufferCrane::takeContainer()
{
    this->mtx.lock();
    this->state = "waiting";
    this->mtx.unlock();
    buffer->work_mtx.lock();
    this->mtx.lock();
    buffer->mtx.lock();
    while (buffer->containerList.size() == 0)
    {
        buffer->mtx.unlock();
        this->mtx.unlock();
        buffer->work_mtx.unlock();
        workSimulation(1);
        buffer->work_mtx.lock();
        this->mtx.lock();
        buffer->mtx.lock();
    }
    currentContainer = buffer->containerList.at(0);
    buffer->containerList.erase(buffer->containerList.begin());
    this->state = " taking container " + to_string(currentContainer->id);
    buffer->mtx.unlock();
    this->mtx.unlock();
    workSimulation(10);
    buffer->work_mtx.unlock();
}

void BufferCrane::putToCar()
{
    this->mtx.lock();
    //this->myCar = NULL;
    while (this->myCar == NULL || (this->myCar != NULL && this->myCar->currentContainer != NULL))
    {
        this->state = "waiting car";
        this->mtx.unlock();
        workSimulation(10);
        this->mtx.lock();
    }
    this->state = "put to car" + to_string(currentContainer->id);
    this->mtx.unlock();
    workSimulation(10);
    this->mtx.lock();
    this->myCar->takeContainer(currentContainer);
    int id = currentContainer->id;
    currentContainer = NULL;
    this->mtx.unlock();
}

void BufferCrane::registerCar(Car *car)
{
    this->myCar = car;
}

void BufferCrane::unregisterCar()
{
    this->myCar = NULL;
}

void BufferCrane::workSimulation(int times)
{
    if (!Port::isRunning)
    {
        this->mtx.unlock();
        return;
    }
    for (int i = 0; i < times; i++)
    {
        this_thread::sleep_for(chrono::milliseconds(100));
        progress = float(i * 100) / times;
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    progress = 0;
}