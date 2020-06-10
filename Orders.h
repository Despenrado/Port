#pragma once
#include "Container.h"

using namespace std;

class Orders
{
private:
    /* data */
public:
    static mutex mtx;
    static vector<Container *> containerList;
    static void lifeCycle();
    static void genereteContainer();
    static bool existContainer(int);
    static Container *giveContainer();
    static void genContainerList(int);
    static void workSimulation(int);
    static void delContainer(int);

    Orders();
};
