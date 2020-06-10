#include "Orders.h"
#include <iostream>

Orders::Orders() {}

void Orders::lifeCycle()
{
    while (true)
    {
        //genereteContainer();
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
    //std::cout << "lock_cont2" << std::endl;
    int random = containerList.size() - 1; //= rand() % containerList.size();
    //std::cout << random << std::endl;
    workSimulation(15);
    containerList.at(random)->mtx.lock();
    while (containerList.at(random)->isSend)
    {
        containerList.at(random)->mtx.unlock();
        mtx.unlock();
        workSimulation(1);
        mtx.lock();
        random = rand() % containerList.size();
        std::cout << random << " : " << containerList.size() << std::endl;
        containerList.at(random)->mtx.lock();
        std::cout << random << " : " << containerList.size() << std::endl;
    }
    containerList.at(random)->isSend = true;
    Container *tmp = containerList.at(random);
    containerList.at(random)->mtx.unlock();
    return tmp;
}

void Orders::genContainerList(int n)
{
    for (int i = 0; i < n; i++)
    {
        int randomID = rand() % 100;
        while (existContainer(randomID))
        {
            randomID = rand() % 100;
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
            Container *tmp = containerList.at(i);
            containerList.erase(containerList.begin() + i);
            tmp->mtx.unlock();
            delete (tmp);
            mtx.unlock();
            return;
        }
        containerList.at(i)->mtx.unlock();
    }
    mtx.unlock();
}

void Orders::workSimulation(int times)
{
    for (int i = 0; i < times; i++)
    {
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}