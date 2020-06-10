#include "Container.h"
#include "Orders.h"
#include "Port.h"
#include "Ship.h"
#include "ncurses.h"
#include "stdarg.h"
#include "stddef.h"
#include <cstring>
#include <string>

using namespace std;

// static variables
atomic_bool *Port::isRunning = new atomic_bool(true);
vector<Dock *> Port::dockList = vector<Dock *>();
vector<int> Port::ship_order = vector<int>();
mutex Port::mtx;
mutex Orders::mtx;
vector<Container *> Orders::containerList = vector<Container *>();

// main variables
bool emergencyExit = false;
mutex mainMutex;

//
// Port;
// Orders;
vector<Ship *> shipList = vector<Ship *>();
vector<thread *> shipTreadList = vector<thread *>();
vector<thread *> craneThreadList = vector<thread *>();       //buffer
vector<thread *> bufferCraneThreadList = vector<thread *>(); //bufferCrane
thread *threadContainerGenerator;

void keyboardFunc()
{
    int key = 0;
    do
    {
        key = getch();
        if (key == 'Q')
        {
            emergencyExit = true;
        }
    } while (key != 27 && !emergencyExit);
    *Port::isRunning = false;
}

void initialize()
{
    Orders::genContainerList(10);
    Port::genDockList(1);
    for (int i = 0; i < 1; i++)
    {
        shipList.push_back(new Ship(i));
    }
}

void launchThreads()
{
    threadContainerGenerator = new thread([]() {
        Orders::lifeCycle();
    });
    for (int i = 0; i < shipList.size(); i++)
    {
        mainMutex.lock();
        shipTreadList.push_back(new thread([i]() {
            shipList.at(i)->lifeCycle();
        }));
        mainMutex.unlock();
    }
    //this_thread::sleep_for(chrono::milliseconds(99999999));
    for (int i = 0; i < Port::dockList.size(); i++)
    {
        mainMutex.lock();
        craneThreadList.push_back(new thread([i]() {
            Port::dockList.at(i)->shipCrane->lifeCycle();
        }));
        bufferCraneThreadList.push_back(new thread([i]() {
            Port::dockList.at(i)->bufferCrane->lifeCycle();
        }));
        mainMutex.unlock();
    }
}

int main(int argc, char **argv)
{
    system("export TERM=xterm-256color");
    srand(time(NULL));

    initialize();
    launchThreads();

    int row, col;
    initscr();
    noecho();
    thread keyboardThread(keyboardFunc);
    raw();
    string help1 = "use ESC to exit;";
    string help2 = "use Shift+Q to emergency stop;";
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE); //main background
    init_pair(4, COLOR_BLACK, COLOR_BLUE);  //frame color
    init_pair(2, COLOR_GREEN, COLOR_WHITE);
    init_pair(3, COLOR_RED, COLOR_WHITE);
    int horizontalOffset = 2;
    int verticalOffset = 1;
    int fps = 0;
    int container_free;
    int container_busy;
    vector<string> ship_id;
    vector<string> ship_status;
    vector<string> ship_cont_listSize;
    vector<string> dock_ship_cont_listSize;
    vector<string> dock_ship_id;
    vector<string> dock_ship_status;
    vector<string> dock_shipCrane_status;
    vector<string> dock_buffer_amount;
    vector<string> dock_bufferCrane_status;
    do
    {
        ////////////////////////////////////////////////////////////////////////////////////////
        container_busy = 0;
        container_free = 0;
        for (int i = 0; i < Orders::containerList.size(); i++)
        {
            Orders::containerList.at(i)->mtx.lock();
            if (Orders::containerList.at(i)->isSend)
            {
                container_busy++;
            }
            else
            {
                container_free++;
            }

            Orders::containerList.at(i)->mtx.unlock();
        }
        for (int i = 0; i < shipList.size(); i++)
        {
            shipList.at(i)->mtx.lock();
            string tmp = shipList.at(i)->state;
            std::cout << "main1" << std::endl;
            if (tmp != "unloading")
            {
                ship_id.push_back(to_string(shipList.at(i)->id));
                ship_status.push_back(tmp);
                ship_cont_listSize.push_back(to_string(shipList.at(i)->containerList.size()));
            }
            else
            {
                ship_id.push_back("");
                ship_status.push_back("");
                ship_cont_listSize.push_back("");
            }
            shipList.at(i)->mtx.unlock();
        }
        for (int i = 0; i < Port::dockList.size(); i++)
        {
            std::cout << "main9" << std::endl;
            Port::dockList.at(i)->bufferCrane->mtx.lock();
            dock_bufferCrane_status.push_back(Port::dockList.at(i)->bufferCrane->state);
            std::cout << "main2" << std::endl;
            Port::dockList.at(i)->bufferCrane->mtx.unlock();
            std::cout << "main5" << std::endl;
            Port::dockList.at(i)->shipCrane->mtx.lock();
            std::cout << "main6" << std::endl;
            dock_shipCrane_status.push_back(Port::dockList.at(i)->shipCrane->state);
            std::cout << "main4" << std::endl;
            Port::dockList.at(i)->shipCrane->dockBuffer->mtx.lock();
            std::cout << "main7" << std::endl;
            dock_buffer_amount.push_back(to_string(Port::dockList.at(i)->shipCrane->dockBuffer->containerList.size()));
            std::cout << "main3" << std::endl;
            Port::dockList.at(i)->shipCrane->dockBuffer->mtx.unlock();
            std::cout << "main8" << std::endl;
            if (Port::dockList.at(i)->ship != NULL)
            {
                Port::dockList.at(i)->ship->mtx.lock();
                dock_ship_status.push_back(Port::dockList.at(i)->ship->state);
                dock_ship_id.push_back(to_string(Port::dockList.at(i)->ship->id));
                dock_ship_cont_listSize.push_back(to_string(Port::dockList.at(i)->ship->containerList.size()));
                Port::dockList.at(i)->ship->mtx.unlock();
            }
            else
            {
                dock_ship_status.push_back("");
                dock_ship_id.push_back("");
                dock_ship_cont_listSize.push_back("");
            }
            Port::dockList.at(i)->shipCrane->mtx.unlock();
        }
        ////////////////////////////////////////////////////////////////////////////////////////
        getmaxyx(stdscr, row, col);
        if (fps >= INT32_MAX)
        {
            fps = 0;
        }
        fps++;
        clear();
        attron(COLOR_PAIR(1));
        for (int i = verticalOffset; i < row - verticalOffset; i++)
        {
            mvhline(i, horizontalOffset, ' ', col - horizontalOffset);
        }
        mvwaddstr(stdscr, verticalOffset, horizontalOffset, to_string(fps).c_str());
        mvwaddstr(stdscr, verticalOffset + 1, horizontalOffset + 2, "in sea:");
        mvvline(verticalOffset + 1, horizontalOffset + 1, '|', row - verticalOffset - 3);
        mvvline(verticalOffset + 1, horizontalOffset + 12, '|', row - verticalOffset - 3);
        // mvvline(verticalOffset + 1, horizontalOffset + 13, '+', row - verticalOffset - 3);
        for (int i = verticalOffset + 2; i <= row - verticalOffset - 3; i += 4)
        {
            mvwaddstr(stdscr, i, horizontalOffset + 2, "----------");
        }
        mvwaddstr(stdscr, verticalOffset + 3, horizontalOffset + 2, "sheep id");
        mvwaddstr(stdscr, verticalOffset + 4, horizontalOffset + 2, "status");
        mvwaddstr(stdscr, verticalOffset + 5, horizontalOffset + 2, "conteiners");

        mvwaddstr(stdscr, verticalOffset + 1, horizontalOffset + 24, "Dock:");
        mvvline(verticalOffset + 1, horizontalOffset + 14, '|', row - verticalOffset - 3);
        mvvline(verticalOffset + 2, horizontalOffset + 25, '|', row - verticalOffset - 3);
        mvvline(verticalOffset + 2, horizontalOffset + 35, '|', row - verticalOffset - 3);
        mvvline(verticalOffset + 1, horizontalOffset + 45, '|', row - verticalOffset - 3);
        for (int i = verticalOffset + 2; i <= row - verticalOffset - 3; i += 10)
        {
            mvwaddstr(stdscr, i, horizontalOffset + 15, "------------------------------");
            mvwaddstr(stdscr, i + 5, horizontalOffset + 26, "-------------------");
        }
        mvwaddstr(stdscr, verticalOffset + 3, horizontalOffset + 15, "sheep id");
        mvwaddstr(stdscr, verticalOffset + 4, horizontalOffset + 15, "status");
        mvwaddstr(stdscr, verticalOffset + 5, horizontalOffset + 15, "conteiners");
        mvwaddstr(stdscr, verticalOffset + 3, horizontalOffset + 26, "ship");
        mvwaddstr(stdscr, verticalOffset + 4, horizontalOffset + 26, "crane");
        mvwaddstr(stdscr, verticalOffset + 3, horizontalOffset + 36, "car");
        mvwaddstr(stdscr, verticalOffset + 8, horizontalOffset + 26, "buffer:");
        mvwaddstr(stdscr, verticalOffset + 9, horizontalOffset + 26, "size");
        mvwaddstr(stdscr, verticalOffset + 8, horizontalOffset + 36, "buffer");
        mvwaddstr(stdscr, verticalOffset + 9, horizontalOffset + 36, "crane");

        mvvline(verticalOffset + 1, horizontalOffset + 46, '|', row - verticalOffset - 3);
        mvvline(verticalOffset + 1, horizontalOffset + 100, '|', row - verticalOffset - 3);

        mvwaddstr(stdscr, verticalOffset + 1, horizontalOffset + 112, "TrainStation:");
        mvvline(verticalOffset + 1, horizontalOffset + 101, '|', row - verticalOffset - 3);
        mvvline(verticalOffset + 2, horizontalOffset + 111, '|', row - verticalOffset - 3);
        mvvline(verticalOffset + 2, horizontalOffset + 121, '|', row - verticalOffset - 3);
        mvvline(verticalOffset + 1, horizontalOffset + 132, '|', row - verticalOffset - 3);
        // mvvline(verticalOffset + 1, horizontalOffset + 133, '+', row - verticalOffset - 3);
        for (int i = verticalOffset + 2; i <= row - verticalOffset - 3; i += 5)
        {
            mvwaddstr(stdscr, i, horizontalOffset + 102, "------------------------------");
        }
        mvwaddstr(stdscr, verticalOffset + 3, horizontalOffset + 122, "sheep id");
        mvwaddstr(stdscr, verticalOffset + 4, horizontalOffset + 122, "status");
        mvwaddstr(stdscr, verticalOffset + 5, horizontalOffset + 122, "conteiners");
        mvwaddstr(stdscr, verticalOffset + 3, horizontalOffset + 112, "train");
        mvwaddstr(stdscr, verticalOffset + 4, horizontalOffset + 112, "crane");
        mvwaddstr(stdscr, verticalOffset + 3, horizontalOffset + 102, "car");

        mvwaddstr(stdscr, verticalOffset + 1, horizontalOffset + 135, "in trevel:");
        mvvline(verticalOffset + 1, horizontalOffset + 134, '|', row - verticalOffset - 3);
        mvvline(verticalOffset + 1, horizontalOffset + 145, '|', row - verticalOffset - 3);
        for (int i = verticalOffset + 2; i <= row - verticalOffset - 3; i += 4)
        {
            mvwaddstr(stdscr, i, horizontalOffset + 135, "----------");
        }
        mvwaddstr(stdscr, verticalOffset + 3, horizontalOffset + 135, "train id");
        mvwaddstr(stdscr, verticalOffset + 4, horizontalOffset + 135, "status");
        mvwaddstr(stdscr, verticalOffset + 5, horizontalOffset + 135, "conteiners");
        //
        //////////////////////////////////////////////////////////////
        //
        mvwaddstr(stdscr, verticalOffset + 1, horizontalOffset + 50, ("reg " + to_string(Port::ship_order.size())).c_str());
        mvwaddstr(stdscr, verticalOffset + 1, horizontalOffset + 60, ("cont_free " + to_string(container_free)).c_str());
        mvwaddstr(stdscr, verticalOffset + 1, horizontalOffset + 80, ("cont_busy " + to_string(container_busy)).c_str());
        for (int i = 0, i_v = verticalOffset + 2 + 4 + 1; i < ship_id.size(); i++, i_v += 4)
        {
            mvwaddstr(stdscr, i_v, horizontalOffset + 2, ship_id.at(i).c_str());
            mvwaddstr(stdscr, i_v + 1, horizontalOffset + 2, ship_status.at(i).c_str());
            mvwaddstr(stdscr, i_v + 2, horizontalOffset + 2, ship_cont_listSize.at(i).c_str());
        }
        for (int i = 0, i_v = verticalOffset + 10; i < dock_shipCrane_status.size(); i++, i_v += 10)
        {
            mvwaddstr(stdscr, i_v + 3, horizontalOffset + 15, dock_ship_id.at(i).c_str());
            mvwaddstr(stdscr, i_v + 4, horizontalOffset + 15, dock_ship_status.at(i).c_str());
            mvwaddstr(stdscr, i_v + 5, horizontalOffset + 15, dock_ship_cont_listSize.at(i).c_str());
            mvwaddstr(stdscr, i_v + 3, horizontalOffset + 26, dock_shipCrane_status.at(i).c_str());
            mvwaddstr(stdscr, i_v + 3, horizontalOffset + 36, "car");
            mvwaddstr(stdscr, i_v + 8, horizontalOffset + 26, dock_buffer_amount.at(i).c_str());
            mvwaddstr(stdscr, i_v + 8, horizontalOffset + 36, dock_bufferCrane_status.at(i).c_str());
        }
        //
        //////////////////////////////////////////////////////////////
        //
        attron(COLOR_PAIR(4));
        for (int i = 0; i < verticalOffset; i++)
        {
            mvhline(i, 0, ' ', col);
        }
        for (int i = row - verticalOffset; i < row; i++)
        {
            mvhline(i, 0, ' ', col);
        }
        for (int i = 0; i < horizontalOffset; i++)
        {
            mvvline(verticalOffset, i, ' ', row - horizontalOffset);
        }
        for (int i = col - horizontalOffset; i < col; i++)
        {
            mvvline(verticalOffset, i, ' ', row - horizontalOffset);
        }
        refresh();
        this_thread::sleep_for(chrono::milliseconds(30));
        ship_id.clear();
        ship_status.clear();
        ship_cont_listSize.clear();
        dock_ship_cont_listSize.clear();
        dock_ship_id.clear();
        dock_ship_status.clear();
        dock_shipCrane_status.clear();
        dock_buffer_amount.clear();
        dock_bufferCrane_status.clear();
    } while (Port::isRunning->load());

    //keyboardThread.~thread();
    clear();
    endwin();
    system("clear");
    system("export TERM=xterm-256color");
    keyboardThread.join();
    if (emergencyExit)
    {
    }
    //cout << "waiting for threads:" << endl;

    delete Port::isRunning;
    //cout << "close" << endl;
    //cout << consoleHelp << endl;
    return 0;
}