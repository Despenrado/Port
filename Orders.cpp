#include "Orders.h"
#include "Port.h"
#include <iostream>

Orders::Orders() {}

void Orders::lifeCycle()
{
    while (Port::isRunning)
    {
        genereteContainer();
    }
}

void Orders::genereteContainer()
{
    //std::cout << "lock_cont10" << std::endl;
    int randomID = rand() % 1000;
    workSimulation(10);
    mtx.lock();
    //std::cout << "lock_cont1" << std::endl;
    while (existContainer(randomID))
    {
        mtx.unlock();
        randomID = rand() % 1000;
        workSimulation(5);
        mtx.lock();
    }
    containerList.push_back(new Container(randomID));
    //std::cout << containerList.size() << " : " << randomID << std::endl;
    mtx.unlock();
}

bool Orders::existContainer(int id)
{
    for (int i = 0; i < containerList.size(); i++)
    {
        containerList.at(i)->mtx.lock();
        if (containerList.at(i)->id == id)
        {
            containerList.at(i)->mtx.unlock();
            return true;
        }
        containerList.at(i)->mtx.unlock();
    }
    return false;
}

Container *Orders::giveContainer()
{
    workSimulation(15);
    mtx.lock();
    int random = rand() % containerList.size();
    //std::cout << random << std::endl;
    containerList.at(random)->mtx.lock();
    while (containerList.at(random)->isSend)
    {
        containerList.at(random)->mtx.unlock();
        mtx.unlock();
        workSimulation(1);
        mtx.lock();
        random = rand() % containerList.size();
        containerList.at(random)->mtx.lock();
    }
    containerList.at(random)->isSend = true;
    Container *tmp = containerList.at(random);
    containerList.at(random)->mtx.unlock();
    mtx.unlock();
    return tmp;
}

void Orders::genContainerList(int n)
{
    for (int i = 0; i < n; i++)
    {
        int randomID = rand() % 1000;
        while (existContainer(randomID))
        {
            randomID = rand() % 1000;
        }
        containerList.push_back(new Container(randomID));
    }
}

void Orders::delContainer(int id)
{
    mtx.lock();
    for (int i = 0; i < containerList.size(); i++)
    {
        containerList.at(i)->mtx.lock();
        if (containerList.at(i)->id == id)
        {
            containerList.erase(containerList.begin() + i);
            mtx.unlock();
            return;
        }
        containerList.at(i)->mtx.unlock();
    }
    mtx.unlock();
}

int Orders::getOrder()
{
    mtx.lock();
    int random = rand() % containerList.size();
    containerList.at(random)->mtx.lock();
    while (!containerList.at(random)->isSend || containerList.at(random)->isOrderd)
    {
        containerList.at(random)->mtx.unlock();
        mtx.unlock();
        workSimulation(1);
        mtx.lock();
        random = rand() % containerList.size();
        containerList.at(random)->mtx.lock();
    }
    containerList.at(random)->isOrderd = true;
    int tmp = containerList.at(random)->id;
    containerList.at(random)->mtx.unlock();
    mtx.unlock();
    return tmp;
}

void Orders::workSimulation(int times)
{
    if (!Port::isRunning)
    {
        mtx.unlock();
        return;
    }
    for (int i = 0; i < times; i++)
    {
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}