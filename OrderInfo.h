#pragma once

using namespace std;

class OrderInfo
{
private:
    /* data */
public:
    int containerId;
    int bufferId;
    int railwayId;

    OrderInfo();
    OrderInfo(int, int, int);
};