#include "TrainCrane.h"
#include "Port.h"

TrainCrane::TrainCrane()
{
    this->myTrain = NULL;
    this->myCar = NULL;
    this->currentContainer = NULL;
}

void TrainCrane::lifeCycle()
{
    while (Port::isRunning)
    {
        waitingForTrain();
        if (!Port::isRunning)
        {
            this->mtx.unlock();
            return;
        }
        putContainerToTrain();
    }
}

// void TrainCrane::unloadCar()
// {
//     if (myCar != NULL)
//     {
//         myCar->mtx.lock();
//         while (myTrain->containerList.size() < myTrain->maxContainers)
//         {
//             if (myCar->currentContainer != NULL)
//             {
//                 myCar->mtx.unlock();
//                 this->mtx.unlock();
//                 takeContainer();
//                 putContainerToTrain();
//                 this->mtx.lock();
//                 if (myCar != NULL)
//                 {
//                     myCar->mtx.lock();
//                 }
//             }
//         }
//         if (myCar != NULL)
//         {
//             myCar->mtx.unlock();
//         }
//         this->mtx.unlock();
//         return;
//     }
//     this->mtx.unlock();
// }

void TrainCrane::waitingForTrain()
{
    this->mtx.lock();
    this->state = "waiting for train";
    while (myTrain == NULL)
    {
        this->mtx.unlock();
        workSimulation(rand() % 10 + 15);
        this->mtx.lock();
    }
}

void TrainCrane::putContainerToTrain()
{
    while (myTrain != NULL && myTrain->containerList.size() < myTrain->maxContainers)
    {
        waitingForCar();
        takeContainer();
        this->mtx.lock();
        if (this->currentContainer == NULL)
        {
            continue;
        }
        myTrain->containerList.push_back(currentContainer);
        this->currentContainer = NULL;
    }
    this->mtx.unlock();
}

void TrainCrane::waitingForCar()
{
    this->state = "waiting for car";
    while (myCar == NULL)
    {
        this->mtx.unlock();
        workSimulation(rand() % 10 + 15);
        this->mtx.lock();
    }
}

void TrainCrane::takeContainer()
{
    this->state = "taking container";
    if (myCar != NULL)
    {
        this->currentContainer = myCar->giveContainer();
        myCar->currentContainer = NULL;
        if (this->currentContainer == NULL)
        {
            this->mtx.unlock();
            return;
        }
        this->state = "taking container:" + to_string(currentContainer->id);
    }
    this->mtx.unlock();
    workSimulation(rand() % 10 + 15);
}

void TrainCrane::workSimulation(int times)
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