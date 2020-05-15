#include "Buffer.h"
#include "Dock.h"


Buffer::Buffer(Dock *dock) {
    this->myDock = dock;
}

void Buffer::lifeCycle(){
    while (true)
    {
        waitingForShip();
        unloadShip();
    }
    
}

void Buffer::unloadShip(){
    if(myDock->ship != NULL){
        while (myDock->ship->containerList.size() > 0)
        {
            takeContainer();
        }
    }
    
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
    while (myDock->ship == NULL){
        workSimulation(rand() % 10 + 15);
    }
}

void Buffer::workSimulation(int times)
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