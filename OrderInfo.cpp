#include "OrderInfo.h"

OrderInfo::OrderInfo() {}
OrderInfo::OrderInfo(int id, int buf, int rail)
{
    this->bufferId = buf;
    this->railwayId = rail;
    this->containerId = id;
}