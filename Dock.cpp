#include "Dock.h"



Dock::Dock() {
    this->buffer = new Buffer (this);
}

void Dock::registerShipInDock(Ship *ship){
    this->ship = ship;
}