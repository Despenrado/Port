#include <vector>
#include <iostream>
#include <stdlib.h>
#include <mutex>

#include "Container.h" 

using namespace std;


class Orders
{
private:
    /* data */
public :
    static mutex mtx;
    static vector<Container *> containerList;
    static void lifeCycle();
    static void genereteContainer();
    static bool existContainer(int);
    static Container* giveContainer();
    


    Orders();

};
