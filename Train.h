#include <iostream>
#include <atomic>
#include <vector>
#include "Container.h"


using namespace std;


class Train
{
    
public:
    int id;
    bool isFull();
    atomic<string> state;
    vector<Container *> containerList;

    Train();

    void stopAtStation();
    void leftFromStation();
    void ride();
    void unloadContainers();
    void loadContainers();
    void takeContainer(Container &);
    void giveContainer(Container &);



};
