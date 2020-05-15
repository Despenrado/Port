#include <vector>
#include <iostream>
#include <stdlib.h>
#include "Dock.h"

using namespace std;

class Port
{
public:
    static vector<Dock *> dockList;

    static bool registerShip(Ship *);
};