#include "Train.h"
#include "Orders.h"
#include "Port.h"

Train::Train() {}

Train::Train(int id)
{
    this->maxContainers = 1;
    this->id = id;
}

void Train::lifeCycle()
{
    while (Port::isRunning)
    {
        if (!Port::isRunning)
        {
            this->mtx.unlock();
            return;
        }
        takeOrders();
        if (!Port::isRunning)
        {
            this->mtx.unlock();
            return;
        }
        ride();
        if (!Port::isRunning)
        {
            this->mtx.unlock();
            return;
        }
        stopAtStation();
        if (!Port::isRunning)
        {
            this->mtx.unlock();
            return;
        }
        loadContainers();
        if (!Port::isRunning)
        {
            this->mtx.unlock();
            return;
        }
        leftFromStation();
        if (!Port::isRunning)
        {
            this->mtx.unlock();
            return;
        }
        ride();
        if (!Port::isRunning)
        {
            this->mtx.unlock();
            return;
        }
        unloadContainers();
    }
}

void Train::takeOrders()
{
    this->mtx.lock();
    this->state = "taking orders";
    while (maxContainers >= this->trainOrderList.size())
    {
        this->mtx.unlock();
        int tmp = Orders::getOrder();
        workSimulation(1);
        this->mtx.lock();
        trainOrderList.push_back(tmp);
    }
    this->mtx.unlock();
    workSimulation(rand() % 10 + 15);
}

void Train::stopAtStation()
{
    this->mtx.lock();
    this->state = "register";
    this->mtx.unlock();
    while (!Port::registerTrain(this))
    {
        workSimulation(rand() % 10 + 15);
    }
}

void Train::leftFromStation()
{
    workSimulation(rand() % 10 + 15);
    do
    {
        this->mtx.lock();
        this->state = "unregister";
        this->mtx.unlock();
        workSimulation(rand() % 10 + 15);
    } while (!Port::unregisterTrain(this));
}

void Train::ride()
{
    this->mtx.lock();
    this->state = "ride";
    this->mtx.unlock();
    workSimulation(rand() % 10 + 15);
}

void Train::unloadContainers()
{
    this->mtx.lock();
    this->state = "unloading";
    while (containerList.size() > 0)
    {
        this->mtx.unlock();
        workSimulation(rand() % 10 + 15);
        this->mtx.lock();
        for (int i = 0; i < this->containerList.size(); i++)
        {
            Orders::delContainer(containerList.at(i)->id);
        }
        containerList.clear();
    }
    this->trainOrderList.clear();
    this->mtx.unlock();
}

void Train::loadContainers()
{
    this->mtx.lock();
    this->state = "loading";
    while (containerList.size() < maxContainers)
    {
        this->mtx.unlock();
        workSimulation(rand() % 10 + 15);
        this->mtx.lock();
    }
    this->mtx.unlock();
}

void Train::takeContainer(Container *container)
{
    this->containerList.push_back(container);
}

Container *Train::giveContainer()
{
    Container *tmp = this->containerList.at(this->containerList.size() - 1);
    this->containerList.pop_back();
    return tmp;
}

void Train::workSimulation(int times)
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