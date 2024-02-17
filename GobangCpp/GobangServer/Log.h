#pragma once
#include <iostream>
#include <ctime>
#include "CommonMethod.h"

using namespace std;

class Log {
public:
    static void Info(const char* message) {
        /*time_t now = time(0);
        tm* dt = localtime(&now);
        cout << "[" << dt->tm_year << "-" << dt->tm_mon << "-" << dt->tm_mday << " " << dt->tm_hour << ":" << dt->tm_min << ":" << dt->tm_sec << "] " << message << endl;*/

        SYSTEMTIME time;
        GetLocalTime(&time);
        printf("[%04d/%02d/%02d %02d:%02d:%02d] %s\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, message);
    }

    static void Info(const char* message, char* param) {
        /*time_t now = time(0);
        tm* dt = localtime(&now);
        cout << "[" << dt->tm_year << "-" << dt->tm_mon << "-" << dt->tm_mday << " " << dt->tm_hour << ":" << dt->tm_min << ":" << dt->tm_sec << "] " << message << param << endl;*/

        SYSTEMTIME time;
        GetLocalTime(&time);
        printf("[%04d/%02d/%02d %02d:%02d:%02d] %s%s\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, message, param);
    }

    static void Info(const char* message, unsigned short param){
        /*time_t now = time(0);
        tm* dt = localtime(&now);
        cout << "[" << dt->tm_year << "-" << dt->tm_mon << "-" << dt->tm_mday << " " << dt->tm_hour << ":" << dt->tm_min << ":" << dt->tm_sec << "] " << message << param << endl;*/

        SYSTEMTIME time;
        GetLocalTime(&time);
        printf("[%04d/%02d/%02d %02d:%02d:%02d] %s%hu\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, message, param);
    }

    static void Info(const char* message, SOCKADDR_IN param){
        char* ip = CommonMethod::AddrToStr(param);

        /*time_t now = time(0);
        tm* dt = localtime(&now);
        cout << "[" << dt->tm_year << "-" << dt->tm_mon << "-" << dt->tm_mday << " " << dt->tm_hour << ":" << dt->tm_min << ":" << dt->tm_sec << "] " << message << ip << endl;*/

        SYSTEMTIME time;
        GetLocalTime(&time);
        printf("[%04d/%02d/%02d %02d:%02d:%02d] %s%s\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, message, ip);
    }
};
