#include "Orders.h"


Orders::Orders() {}

void Orders::lifeCycle(){
    while (true)
    {
        genereteContainer();
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
