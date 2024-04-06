#include <iostream>
#include <thread>
#include "GameStart.h"
#include <mutex>
#include <easyx.h>
#include "Robot.h"

using namespace std;

std::mutex mtx;

static void ExcuteT(GameStart* client) {
    client->Start(&mtx);
}

int main() {
    GameStart app1;
    //GameStart app2;

    thread t1(ExcuteT, &app1);
    //thread t2(ExcuteT, &app2);
    Robot r1;
    r1.JoinGame();
    getchar();
}