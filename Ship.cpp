#include "Ship.h"
#include "Port.h"

Ship::Ship(int id) {
    this->maxContainers = 5;
    this->id = id;
}

Ship::Ship(){}

void Ship::lifeCycle(){
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

void Ship::loadContainers(){

    this->state = "loading";
    while (maxContainers >= containerList.size())
    {
        takeContainer();
    }
}

void Ship::unloadContainers(){
    this->state = "unloading";
    workSimulation(rand() % 10 + 15);
    while (containerList.size() > 0)
    {
        workSimulation(rand() % 10 + 15);
    }
}

Container* Ship::giveContainer(){
    Container *tmp = this->containerList.at(this->containerList.size()-1);
    this->containerList.pop_back();
    return tmp;
}

void Ship::takeContainer(){
    this->state = "taking conteiner";
    this->mtx.lock();
    containerList.push_back(Orders::giveContainer());
    this->mtx.unlock();
}

void Ship::sail(){
    this->state = "sailing";
    workSimulation(rand() % 10 + 15);
}

void Ship::moor(){
    this->state = "mooring";
    workSimulation(rand() % 10 + 15);
    Port::registerShip(this);
}

void Ship::unMoor(){
    this->state = "unmooring";
    workSimulation(rand() % 10 + 15);
    // докер, удали на меня указатель
}

void Ship::workSimulation(int times)
{
    for (int i = 0; i < times; i++)
    {
        this_thread::sleep_for(chrono::milliseconds(1));
        this->mtx.lock();
        progress = float(i * 100) / times;
        this->mtx.unlock();
        this_thread::sleep_for(chrono::milliseconds(1));
    }
    this->mtx.lock();
    progress = 0;
    this->mtx.unlock();
}
