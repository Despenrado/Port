#include "Ship.h"


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
    while (maxContainers > containerList.size())
    {
        takeContainer();
    }
}

void Ship::unloadContainers(){
    this->state = "unloading";
    workSimulation(rand() % 10 + 15);
    containerList.clear();
    // ждем пока кран отпустит
}

void Ship::takeContainer(){
    this->state = "taking conteiner";
    containerList.push_back(Orders::giveContainer());
}

void Ship::sail(){
    this->state = "sailing";
    workSimulation(rand() % 10 + 15);
}

void Ship::moor(){
    this->state = "sailing";
    workSimulation(rand() % 10 + 15);
    // передать докеру указатель на себя
}

void Ship::unMoor(){
    this->state = "sailing";
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
