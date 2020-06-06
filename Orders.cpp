#include "Orders.h"
#include <iostream>

Orders::Orders() {}

void Orders::lifeCycle()
{
    while (true)
    {
        genereteContainer();
    }
}

void Orders::genereteContainer()
{
    //std::cout << "lock_cont10" << std::endl;
    int randomID = rand() % 100;
    workSimulation(10);
    mtx.lock();
    //std::cout << "lock_cont1" << std::endl;
    while (existContainer(randomID))
    {
        mtx.unlock();
        randomID = rand() % 100;
        workSimulation(10);
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
        if (containerList.at(i)->id == id)
        {
            return true;
        }
    }
    return false;
}

Container *Orders::giveContainer()
{
    //std::cout << "lock_cont20" << std::endl;
    mtx.lock();
    //std::cout << "lock_cont2" << std::endl;
    int random = rand() % containerList.size();
    //std::cout << random << std::endl;
    workSimulation(15);
    while (containerList.at(random)->isSend)
    {
        mtx.unlock();
        random = rand() % containerList.size();
        //std::cout << random << " : " << containerList.size() << std::endl;
        workSimulation(1);
        mtx.lock();
    }
    containerList.at(random)->isSend = true;
    Container *tmp = containerList.at(random);
    mtx.unlock();
    return tmp;
}

void Orders::genContainerList(int n)
{
    for (int i = 0; i < n; i++)
    {
        Orders::genereteContainer();
    }
}

void Orders::workSimulation(int times)
{
    for (int i = 0; i < times; i++)
    {
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}