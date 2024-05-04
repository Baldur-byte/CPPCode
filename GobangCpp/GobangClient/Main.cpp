#include <iostream>
#include <thread>
#include "GameStart.h"
#include <mutex>
#include <easyx.h>

using namespace std;

std::mutex mtx;

static void ExcuteT(GameStart* client) {
    client->Start(&mtx);
}

int main() {
    GameStart app1;
    thread t1(ExcuteT, &app1);
    t1.join();
    return 0;
}