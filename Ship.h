#include <iostream>
#include <vector>
#include <atomic>
#include "Container.h"



using namespace std;

class Ship
{
private:
    /* data */
public:
    int id;
    bool isFull;
    atomic<string> state;
    vector<Container *> containerList;

    Ship();

    void moor();
    void unMoor();
    void sail();
    void unloadContainers();
    void loadContainers();
    void takeContainer(Container &);
    void giveContainer(Container &);
    

};
