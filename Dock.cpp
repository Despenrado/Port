#include "Dock.h"

Dock::Dock()
{
    this->shipCrane = new ShipCrane();
}

void Dock::registerShipInDock(Ship *ship)
{
    this->shipCrane->myShip = ship;
    this->ship = ship;
}

void Dock::unregisterShipInDock()
{
    this->shipCrane->myShip = NULL;
    this->ship = NULL;
}