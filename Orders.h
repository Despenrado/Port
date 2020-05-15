#include <vector>
#include <iostream>
#include <stdlib.h>

#include "Container.h" 

using namespace std;


class Orders
{
private:
    /* data */
public :

    static vector<Container *> containerList;
   
    static void genereteContainer();
    static bool existContainer(int);
    static Container* giveContainer();


    Orders();

};
