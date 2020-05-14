#include <iostream>
#include <atomic>
#include "Container.h"


using namespace std;


class Train
{
    
public:
    int id;
    bool isFull();
    atomic<string> state;
    
    
    void takeContainer(Container &);
    void giveContainer(Container &);



};
