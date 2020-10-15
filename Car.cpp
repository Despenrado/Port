#include "Car.h"
#include "Port.h"

Car::Car(int id)
{
    this->id = id;
    this->currentContainer = NULL;
    this->orderInfo = new OrderInfo(-1, -1, -1);
}

Car::Car() {}

void Car::lifeCycle()
{
    int t_id = this->id;
    while (t_id % 2 == 0 && Port::isRunning)
    {
        rideFree();
        if (!Port::isRunning)
        {
            this->mtx.unlock();
            return;
        }
        loadFromDock();
        if (!Port::isRunning)
        {
            this->mtx.unlock();
            return;
        }
        rideFull();
        if (!Port::isRunning)
        {
            this->mtx.unlock();
            return;
        }
        unloadToMainBuffer();
    }
    while (t_id % 2 == 1 && Port::isRunning)
    {
        this->mtx.lock();
        this->state = "take order";
        while (orderInfo->bufferId == -1)
        {
            this->orderInfo = Port::getOrder();
            this->mtx.unlock();
            workSimulation(rand() % 10);
            this->mtx.lock();
        }
        this->mtx.unlock();
        if (!Port::isRunning)
        {
            return;
        }
        rideFree();
        if (!Port::isRunning)
        {
            this->mtx.unlock();
            return;
        }
        loadFromMainBuffer();
        if (!Port::isRunning)
        {
            this->mtx.unlock();
            return;
        }
        rideFull();
        if (!Port::isRunning)
        {
            this->mtx.unlock();
            return;
        }
        unloadToTrain();
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

void Car::loadFromDock()
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
    do
    {
        this->mtx.unlock();
        workSimulation(rand() % 10 + 15);
        this->mtx.lock();
    } while (currentContainer == NULL);
    this->mtx.unlock();
    do
    {
        this->mtx.lock();
        this->state = "unparking";
        this->mtx.unlock();
        workSimulation(rand() % 10 + 15);
    } while (!Port::unregisterDockCar(this));
}

void Car::loadFromMainBuffer()
{
    this->mtx.lock();
    this->state = "loadFromMainBuffer";
    this->mtx.unlock();
    while (!Port::registerMainBufferCarLoad(this))
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
    } while (!Port::unregisterMainBufferCarLoad(this));
}

void Car::unloadToMainBuffer()
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

void Car::unloadToTrain()
{
    this->mtx.lock();
    this->state = "unloadToTrain";
    this->mtx.unlock();
    while (!Port::registerRailWayCar(this)) //
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
    } while (!Port::unregisterRailWayCar(this));
    this->mtx.lock();
    this->orderInfo = new OrderInfo(-1, -1, -1);
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
    if (!Port::isRunning)
    {
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
