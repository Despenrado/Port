#include "Dock.h"

Dock::Dock()
{
    this->buffer = new Buffer();
}

void Dock::registerShipInDock(Ship *ship)
{
    this->buffer->myShip = ship;
    this->ship = ship;
}

void Dock::unregisterShipInDock()
{
    this->ship = NULL;
}