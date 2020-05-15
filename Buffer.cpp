#include "Buffer.h"
#include "Dock.h"
#include "Port.h"


Buffer::Buffer(Dock *dock) {
    this->myDock = dock;
}

void Buffer::lifeCycle(){
    while (Port::isRunning->load())
    {
        waitingForShip();
        unloadShip();
    }
    
}

void Buffer::unloadShip(){
    this->mtx.lock();
    if(myDock->ship != NULL){
        while (myDock->ship->containerList.size() > 0)
        {
            this->mtx.unlock();
            takeContainer();
            this->mtx.lock();
        }
        this->mtx.unlock();
        return;
    }
    this->mtx.unlock();
    
}

void Buffer::takeContainer(){
    this->state = "waiting for ship";
    if(myDock->ship != NULL){
        containerList.push_back(myDock->ship->giveContainer());
    }
    workSimulation(rand() % 10 + 15);

}

void Buffer::waitingForShip(){
    this->state = "waiting for ship";
    this->mtx.lock();
    while (myDock->ship == NULL){
        this->mtx.unlock();
        workSimulation(rand() % 10 + 15);
        this->mtx.lock();
    }
    this->mtx.unlock();
}

void Buffer::workSimulation(int times)
{
    for (int i = 0; i < times; i++)
    {
        this_thread::sleep_for(chrono::milliseconds(1));
        progress = float(i * 100) / times;
        this_thread::sleep_for(chrono::milliseconds(1));
    }
    progress = 0;
}