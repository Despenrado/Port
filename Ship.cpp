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
    while (true)
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
    //std::cout << state << std::endl;
    while (maxContainers >= containerList.size())
    {
        takeContainer();
    }
}

void Ship::unloadContainers()
{
    this->mtx.lock();
    this->state = "unloading";
    //std::cout << state << std::endl;
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
    this->mtx.lock();
    this->state = "taking conteiner";
    this->mtx.unlock();
    //std::cout << state << std::endl;
    Container *tmp = Orders::giveContainer();
    //std::cout << "lock_ship" << std::endl;
    this->mtx.lock();
    this->containerList.push_back(tmp);
    this->mtx.unlock();
}

void Ship::sail()
{
    this->mtx.lock();
    this->state = "sailing";
    this->mtx.unlock();
    //std::cout << state << std::endl;
    workSimulation(rand() % 10 + 15);
}

void Ship::moor()
{
    do
    {
        this->mtx.lock();
        this->state = "waiting for mooring";
        this->mtx.unlock();
        workSimulation(rand() % 10 + 15);
    } while (!Port::registerShip(this));
}

void Ship::unMoor()
{
    //std::cout << state << std::endl;
    workSimulation(rand() % 10 + 15);
    do
    {
        this->mtx.lock();
        this->state = "waiting for unmooring";
        this->mtx.unlock();
        workSimulation(rand() % 10 + 15);
    } while (!Port::unregisterShip(this));
}

void Ship::workSimulation(int times)
{
    for (int i = 0; i < times; i++)
    {
        this_thread::sleep_for(chrono::milliseconds(100));
        progress = float(i * 100) / times;
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    progress = 0;
}
