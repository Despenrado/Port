#include "Orders.h"
#include "Port.h"


Orders::Orders() {}

void Orders::lifeCycle(){
    while (Port::isRunning->load())
    {
        genereteContainer();
        workSimulation(20);
    }   
}


void Orders::genereteContainer(){
    int randomID = rand() % 100 + 1;
    mtx.lock();
    while (existContainer(randomID))
    {
        randomID = rand() % 100 + 1;
    }
    
    containerList.push_back(new Container(rand() % 100 + 1));
    mtx.unlock();
}

bool Orders::existContainer(int id){
    for(int i = 0; i < containerList.size(); i++){
        if(containerList.at(i)->id==id){
            return true;
        }
    }
    return false;
}

Container* Orders::giveContainer(){
    int random = rand() % containerList.size();
    mtx.lock();
    while (containerList.at(random)->isSend)
    {
        random = rand() % 100 + 1;
    }
    containerList.at(random)->isSend = true;
    Container* tmp = containerList.at(random);
    mtx.unlock();
    return tmp;
}

void Orders::genContainerList(int n){
    for(int i = 0; i < n; i++){
        Orders::genereteContainer();
    }
}

void Orders::workSimulation(int times)
{
    for (int i = 0; i < times; i++)
    {
        this_thread::sleep_for(chrono::milliseconds(1));
    }
}