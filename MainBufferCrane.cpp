#include "MainBufferCrane.h"
#include "Orders.h"
#include "Port.h"
#include <sstream>

MainBufferCrane::MainBufferCrane(Buffer *tmp)
{
    this->buffer = tmp;
    this->myCar = NULL;
    this->currentContainer = NULL;
}

void MainBufferCrane::lifeCycle()
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

void MainBufferCrane::takeContainer()
{
    this->mtx.lock();
    while (this->myCar == NULL || (this->myCar != NULL && this->myCar->currentContainer != NULL))
    {
        this->state = "waiting car";
        this->mtx.unlock();
        workSimulation(10);
        this->mtx.lock();
    }
    this->mtx.unlock();
    buffer->work_mtx.lock();
    this->mtx.lock();
    buffer->mtx.lock();
    while (buffer->containerList.size() == 0)
    {
        this->state = "waiting";
        buffer->mtx.unlock();
        this->mtx.unlock();
        buffer->work_mtx.unlock();
        workSimulation(1);
        buffer->work_mtx.lock();
        this->mtx.lock();
        buffer->mtx.lock();
    }
    for (int i = 0; i < buffer->containerList.size(); i++)
    {
        myCar->mtx.lock();
        if (buffer->containerList.at(i)->id == myCar->orderInfo->containerId)
        {
            currentContainer = buffer->containerList.at(i);
            buffer->containerList.at(i)->mtx.unlock();
            buffer->containerList.erase(buffer->containerList.begin() + i);
            myCar->mtx.unlock();
            break;
        }
        myCar->mtx.unlock();
        buffer->containerList.at(i)->mtx.unlock();
    }
    // if (currentContainer == NULL)
    // {
    //     buffer->mtx.unlock();
    //     this->mtx.unlock();
    //     buffer->work_mtx.unlock();
    //     return;
    // }

    // for (int i = 0; i < buffer->containerList.size(); i++)
    // {
    //     buffer->containerList.at(i)->mtx.lock();
    //     if (myCar == NULL || myCar->orderInfo->containerId == -1)
    //     {
    //         currentContainer = buffer->containerList.at(0);
    //         buffer->containerList.at(0)->mtx.unlock();
    //         buffer->containerList.erase(buffer->containerList.begin());
    //         break;
    //     }
    //     else
    //     {
    //         while (this->myCar == NULL || (this->myCar != NULL && this->myCar->currentContainer != NULL))
    //         {
    //             this->state = "waiting car";
    //             this->mtx.unlock();
    //             workSimulation(10);
    //             this->mtx.lock();
    //         }
    //         myCar->mtx.lock();
    //         if (buffer->containerList.at(i)->id == myCar->orderInfo->containerId)
    //         {
    //             currentContainer = buffer->containerList.at(i);
    //             buffer->containerList.at(i)->mtx.unlock();
    //             buffer->containerList.erase(buffer->containerList.begin() + i);
    //             myCar->mtx.unlock();
    //             break;
    //         }
    //         myCar->mtx.unlock();
    //     }

    //     buffer->containerList.at(i)->mtx.unlock();
    // }
    this->state = " taking container " + to_string(currentContainer->id);
    buffer->mtx.unlock();
    this->mtx.unlock();
    workSimulation(10);
    buffer->work_mtx.unlock();
}

void MainBufferCrane::putToCar()
{
    this->mtx.lock();
    this->state = "put to car" + to_string(currentContainer->id);
    this->mtx.unlock();
    workSimulation(10);
    this->mtx.lock();
    this->myCar->takeContainer(currentContainer);
    int id = currentContainer->id;
    currentContainer = NULL;
    this->mtx.unlock();
}

void MainBufferCrane::registerCar(Car *car)
{
    this->myCar = car;
}

void MainBufferCrane::unregisterCar()
{
    this->myCar = NULL;
}

void MainBufferCrane::workSimulation(int times)
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