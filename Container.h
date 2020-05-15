#pragma once
#include <vector>
#include <mutex>
#include <thread>

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
