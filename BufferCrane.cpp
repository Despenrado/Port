#include "BufferCrane.h"
#include "Orders.h"
#include <sstream>

BufferCrane::BufferCrane(Buffer *tmp)
{
    this->buffer = tmp;
    this->myCar = NULL;
    this->currentContainer = NULL;
}

void BufferCrane::lifeCycle()
{
    while (true)
    {
        takeContainer();
        putToCar();
    }
}

void BufferCrane::takeContainer()
{
    this->mtx.lock();
    this->state = "waiting";

    buffer->work_mtx.lock();
    buffer->mtx.lock();
    while (buffer->containerList.size() == 0)
    {
        //std::cout << 111111111111111 << std::endl;
        buffer->mtx.unlock();
        buffer->work_mtx.unlock();
        this->mtx.unlock();
        //std::cout << 222222222222222 << std::endl;
        workSimulation(1);
        //std::cout << 55555555555555 << std::endl;
        buffer->work_mtx.lock();
        this->mtx.lock();
        buffer->mtx.lock();
        //std::cout << "containetList-size" << std::endl;
        //std::cout << 66666666666666 << std::endl;
    }
    //std::cout << 222222222222222 << std::endl;
    currentContainer = buffer->containerList.at(0);
    buffer->containerList.erase(buffer->containerList.begin());
    this->mtx.lock();
    //std::cout << 3333333333333333 << std::endl;
    this->state = " taking container " + to_string(currentContainer->id);
    this->mtx.unlock();
    buffer->mtx.unlock();
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
    for (int i = 0; i < times; i++)
    {
        this_thread::sleep_for(chrono::milliseconds(100));
        progress = float(i * 100) / times;
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    progress = 0;
}