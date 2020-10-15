#include "CarCrane.h"
#include "Port.h"

CarCrane::CarCrane()
{
    buffer = new Buffer();
    this->myCar = NULL;
    this->currentContainer = NULL;
}

void CarCrane::lifeCycle()
{
    while (Port::isRunning)
    {
        waitingForCar();
        if (!Port::isRunning)
        {
            this->mtx.lock();
            return;
        }
        unloadCar();
    }
}

void CarCrane::unloadCar()
{
    if (myCar != NULL)
    {
        myCar->mtx.lock();
        if (myCar->currentContainer != NULL)
        {
            myCar->mtx.unlock();
            this->mtx.unlock();
            takeContainer();
            putContainer();
            this->mtx.lock();
            if (myCar != NULL)
            {
                myCar->mtx.lock();
            }
        }
        if (myCar != NULL)
        {
            myCar->mtx.unlock();
        }
        this->mtx.unlock();
        return;
    }
    this->mtx.unlock();
}

void CarCrane::takeContainer()
{
    this->mtx.lock();
    this->state = "taking container";
    if (myCar != NULL)
    {
        myCar->mtx.lock();
        this->currentContainer = myCar->giveContainer();
        myCar->currentContainer = NULL;
        myCar->mtx.unlock();
        this->state = "taking container:" + to_string(currentContainer->id);
    }
    this->mtx.unlock();
    workSimulation(rand() % 10 + 15);
}

void CarCrane::putContainer()
{
    this->mtx.lock();
    this->state = "putting container:" + to_string(currentContainer->id);
    this->mtx.unlock();
    buffer->work_mtx.lock();
    buffer->mtx.lock();
    this->mtx.lock();
    buffer->containerList.push_back(currentContainer);
    this->mtx.unlock();
    buffer->mtx.unlock();
    workSimulation(rand() % 10 + 15);
    buffer->work_mtx.unlock();
}

void CarCrane::waitingForCar()
{
    this->mtx.lock();
    this->state = "waiting for car";
    while (myCar == NULL)
    {
        this->mtx.unlock();
        workSimulation(rand() % 10 + 15);
        this->mtx.lock();
    }
}

void CarCrane::workSimulation(int times)
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