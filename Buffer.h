#include <iostream>
#include <atomic>
#include <vector>
#include <thread>
#include <mutex>
#include "Container.h"


using namespace std;

class Buffer
{
private:

public:
    atomic<string> state;
    Dock *myDock;
    int progress;
    mutex mtx;
    vector<Container *> containerList;

    void unloadShip();
    void takeContainer();
    void giveContainer(Container &);
    void lifeCycle();
    void waitingForShip();
    void workSimulation(int);


    Buffer(Dock *);



};
