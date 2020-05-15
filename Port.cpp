#include "Port.h"

bool Port::registerShip(Ship *newShip){
    for (int i = 0; i < dockList.size(); i++)
    {
        dockList.at(i)->mtxBusy.lock();
        if(!dockList.at(i)->isBusy){
            dockList.at(i)->registerShipInDock(newShip);
            dockList.at(i)->isBusy=true;
            dockList.at(i)->mtxBusy.unlock();
            return true;
        }
        dockList.at(i)->mtxBusy.unlock();
    }
    return false;
}