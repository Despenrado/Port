#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include "Orders.h"
#include "Container.h"
//#include "Port.h"



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
    int progress;
    mutex mtx;

    Ship();
    Ship(int id);

    void moor();
    void unMoor();
    void sail();
    void unloadContainers();
    void loadContainers();
    void takeContainer();
    Container* giveContainer();
    void lifeCycle();
    void workSimulation(int);
};
