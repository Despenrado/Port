#include "BufferCrane.h"
#include "Orders.h"
#include <sstream>

BufferCrane::BufferCrane(DockBuffer *tmp)
{
    this->dockBuffer = tmp;
}

void BufferCrane::lifeCycle()
{
    while (true)
    {
        // takeContainer();
        // putToCar();
    }
}

void BufferCrane::takeContainer()
{
    dockBuffer->work_mtx.lock();
    dockBuffer->mtx.lock();
    do
    {
        std::cout << 111111111111111 << std::endl;
        this->mtx.lock();
        this->state = "waiting";
        this->mtx.unlock();
        dockBuffer->mtx.unlock();
        dockBuffer->work_mtx.unlock();
        std::cout << 222222222222222 << std::endl;
        //workSimulation(1);
        std::cout << 55555555555555 << std::endl;
        dockBuffer->work_mtx.lock();
        dockBuffer->mtx.lock();
        std::cout << "containetList-size" << std::endl;
        std::cout << 66666666666666 << std::endl;
    } while (dockBuffer->containerList.size() == 0);
    std::cout << 222222222222222 << std::endl;
    currentContainer = dockBuffer->containerList.at(0);
    dockBuffer->containerList.erase(dockBuffer->containerList.begin());
    this->mtx.lock();
    std::cout << 3333333333333333 << std::endl;
    this->state = " taking container " + to_string(currentContainer->id);
    this->mtx.unlock();
    dockBuffer->mtx.unlock();
    workSimulation(20);
    dockBuffer->work_mtx.unlock();
}

void BufferCrane::putToCar()
{
    this->mtx.lock();
    this->state = "put to car" + to_string(currentContainer->id);
    int id = currentContainer->id;
    currentContainer = NULL;
    std::cout << 44444444444444 << std::endl;
    //Orders::delContainer(id);
    std::cout << 44444444444444 << std::endl;
    this->mtx.unlock();
    workSimulation(10);
}

void BufferCrane::workSimulation(int times)
{
    for (int i = 0; i < times; i++)
    {
        this_thread::sleep_for(chrono::milliseconds(10));
        progress = float(i * 100) / times;
        this_thread::sleep_for(chrono::milliseconds(10));
    }
    progress = 0;
}