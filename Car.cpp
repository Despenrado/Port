#include "Car.h"
#include "Port.h"

Car::Car(int id)
{
    this->id = id;
    this->currentContainer = NULL;
}

Car::Car() {}

void Car::lifeCycle()
{
    while (true)
    {
        rideFree();
        loadDock();
        rideFull();
        unloadDock();
    }
}
void Car::rideFree()
{
    this->mtx.lock();
    this->state = "ride free";
    this->mtx.unlock();
    workSimulation(rand() % 10 + 15);
}

void Car::rideFull()
{
    this->mtx.lock();
    this->state = "ride full";
    this->mtx.unlock();
    workSimulation(rand() % 10 + 15);
}

void Car::loadDock()
{
    this->mtx.lock();
    this->state = "waiting";
    this->mtx.unlock();
    while (!Port::registerDockCar(this))
    {
        workSimulation(rand() % 10 + 15);
    }
    this->mtx.lock();
    this->state = "loading";
    while (currentContainer == NULL)
    {
        this->mtx.unlock();
        workSimulation(rand() % 10 + 15);
        this->mtx.lock();
    }
    this->mtx.unlock();
    do
    {
        this->mtx.lock();
        this->state = "unparking";
        this->mtx.unlock();
        workSimulation(rand() % 10 + 15);
    } while (!Port::unregisterDockCar(this));
}

void Car::loadTrain()
{
    this->mtx.lock();
    this->state = "loadTrain";
    this->mtx.unlock();
}

void Car::unloadDock()
{
    this->mtx.lock();
    this->state = "waiting B";
    this->mtx.unlock();
    while (!Port::registerMainBufferCarUnload(this))
    {
        workSimulation(rand() % 10 + 15);
    }
    this->mtx.lock();
    this->state = "unloading";
    while (currentContainer != NULL)
    {
        this->mtx.unlock();
        workSimulation(rand() % 10 + 15);
        //currentContainer = NULL;
        this->mtx.lock();
    }
    this->mtx.unlock();
    do
    {
        this->mtx.lock();
        this->state = "unparking";
        this->mtx.unlock();
        workSimulation(rand() % 10 + 15);
    } while (!Port::unregisterMainBufferCarUnload(this));
}

void Car::unloadTrain()
{
    this->mtx.lock();
    this->state = "unloadTrain";
    this->mtx.unlock();
}

Container *Car::giveContainer()
{
    return this->currentContainer;
}

void Car::takeContainer(Container *container)
{
    this->mtx.lock();
    this->currentContainer = container;
    this->mtx.unlock();
}

void Car::workSimulation(int times)
{
    for (int i = 0; i < times; i++)
    {
        this_thread::sleep_for(chrono::milliseconds(100));
        progress = float(i * 100) / times;
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    progress = 0;
}
