#include "Orders.h"


Orders::Orders() {}


void Orders::genereteContainer(){
    
    int randomID = rand() % 100 + 1;
    while (existContainer(randomID))
    {
        randomID = rand() % 100 + 1;
    }
    
    containerList.push_back(new Container(rand() % 100 + 1));
    
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
     while (containerList.at(random)->isSend)
    {
        random = rand() % 100 + 1;
    }
    containerList.at(random)->isSend = true;
    return containerList.at(random);
    
}
