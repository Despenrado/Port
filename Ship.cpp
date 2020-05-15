#include "Ship.h"
#include "Port.h"

Ship::Ship(int id)
{
    this->maxContainers = 5;
    this->id = id;
}

Ship::Ship() {}

void Ship::lifeCycle()
{
    while (Port::isRunning->load())
    {
        loadContainers();
        sail();
        moor();
        unloadContainers();
        unMoor();
        sail();
    }
}

void Ship::loadContainers()
{

    this->state = "loading";
    while (maxContainers >= containerList.size())
    {
        takeContainer();
    }
}

void Ship::unloadContainers()
{
    this->state = "unloading";
    workSimulation(rand() % 10 + 15);
    this->mtx.lock();
    while (containerList.size() > 0)
    {
        this->mtx.unlock();
        workSimulation(rand() % 10 + 15);
        this->mtx.lock();
    }
    this->mtx.unlock();
}

Container *Ship::giveContainer()
{
    Container *tmp = this->containerList.at(this->containerList.size() - 1);
    this->containerList.pop_back();
    return tmp;
}

void Ship::takeContainer()
{
    this->state = "taking conteiner";
    this->mtx.lock();
    containerList.push_back(Orders::giveContainer());
    this->mtx.unlock();
}

void Ship::sail()
{
    this->state = "sailing";
    workSimulation(rand() % 10 + 15);
}

void Ship::moor()
{
    this->state = "mooring";
    workSimulation(rand() % 10 + 15);
    while (Port::registerShip(this))
    {
        this->state = "waitin for mooring";
        workSimulation(rand() % 10 + 15);
    }
}

void Ship::unMoor()
{
    this->state = "unmooring";
    workSimulation(rand() % 10 + 15);
    while (Port::unregisterShip(this))
    {
        this->state = "waitin for unmooring";
        workSimulation(rand() % 10 + 15);
    }
}

void Ship::workSimulation(int times)
{
    for (int i = 0; i < times; i++)
    {
        this_thread::sleep_for(chrono::milliseconds(1));
        progress = float(i * 100) / times;
        this_thread::sleep_for(chrono::milliseconds(1));
    }
    progress = 0;
}
