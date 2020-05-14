#include "stdarg.h"
#include "stddef.h"
#include "ncurses.h"
#include <cstring>
#include <string>
#include <atomic>
#include <thread>
#include <iostream>
#include <vector>

using namespace std;

atomic<bool> *isRunning = new atomic<bool>(true);
bool emergencyExit = false;

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
    *isRunning = false;
}

int main(int argc, char **argv)
{
    system("export TERM=xterm-256color");
    srand(time(NULL));

    int row, col;
    initscr();
    noecho();
    thread keyboardThread(keyboardFunc);
    raw();
    string help1 = "use ESC to exit;";
    string help2 = "use Shift+Q to emergency stop;";
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(4, COLOR_BLACK, COLOR_BLUE);
    init_pair(2, COLOR_GREEN, COLOR_WHITE);
    init_pair(3, COLOR_RED, COLOR_WHITE);
    int hOffset = 5;
    int vOffset = 3;
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
        for (int i = vOffset; i < row - vOffset; i++)
        {
            mvhline(i, 5, ' ', col - 10);
        }

        attron(COLOR_PAIR(4));
        for (int i = 0; i < vOffset; i++)
        {
            mvhline(i, 0, ' ', col);
        }
        for (int i = row - vOffset; i < row; i++)
        {
            mvhline(i, 0, ' ', col);
        }
        for (int i = 0; i < hOffset; i++)
        {
            mvvline(vOffset, i, ' ', row - hOffset);
        }
        for (int i = col - hOffset; i < col; i++)
        {
            mvvline(vOffset, i, ' ', row - hOffset);
        }
        refresh();
        this_thread::sleep_for(chrono::milliseconds(50));
    } while (isRunning->load());

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

    delete isRunning;
    cout << "close" << endl;
    //cout << consoleHelp << endl;
    return 0;
}