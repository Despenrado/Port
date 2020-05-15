#include <iostream>
#include <vector>
#include <atomic>
#include "Orders.h"
#include "Container.h"



using namespace std;

class Ship
{
private:
    /* data */
public:
    int id;
    atomic<string> state;
    vector<Container *> containerList;
    int maxContainers;

    Ship();
    Ship(int id);

    void moor();
    void unMoor();
    void sail();
    void unloadContainers();
    void loadContainers();
    void takeContainer();
    void giveContainer();
    

};
