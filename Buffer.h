#include <iostream>
#include <atomic>
#include <vector>
#include "Container.h"


using namespace std;

class Buffer
{
private:

public:
    
    vector<Container *> containerList;
    void takeContainer(Container &);
    void giveContainer(Container &);

    Buffer();



};
