#include "stdarg.h"
#include "stddef.h"
#include "ncurses.h"
#include <cstring>
#include <string>
#include "Port.h"
#include "Orders.h"
#include "Container.h"
#include "Dock.h"
#include "Ship.h"

using namespace std;

// static variables
atomic_bool *Port::isRunning = new atomic_bool(true);
vector<Dock *> Port::dockList = vector<Dock *>();
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
vector<thread *> craneThreadList = vector<thread *>(); //buffer
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
        //std::terminate();
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
    for (int i = 0; i < shipList.size(); i++)
    {
        mainMutex.lock();
        shipTreadList.push_back(new thread([i]() {
            shipList.at(i)->lifeCycle();
        }));
        mainMutex.unlock();
    }
    for (int i = 0; i < shipList.size(); i++)
    {
        mainMutex.lock();
        craneThreadList.push_back(new thread([i]() {
            Port::dockList.at(i)->buffer->lifeCycle();
        }));
        mainMutex.unlock();
    }
    threadContainerGenerator = new thread([]() {
        Orders::lifeCycle();
    });
}

int main(int argc, char **argv)
{
    system("export TERM=xterm-256color");
    srand(time(NULL));

    initialize();
    //launchThreads();

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

    do
    {
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
        //////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////
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
        this_thread::sleep_for(chrono::milliseconds(16));
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