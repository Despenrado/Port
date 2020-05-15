#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include "Container.h"



using namespace std;

class Ship
{
private:
    /* data */
public:
    int id;
    atomic<string> state = atomic<string>("");
    vector<Container *> containerList;
    int maxContainers;
    atomic_int progress = atomic_int(0);
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
