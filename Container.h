#pragma once
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

class Container
{
private:
    /* data */
public:
    int id;
    bool isSend;

    Container(int);
    Container();
};
