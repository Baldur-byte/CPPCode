#pragma once
#include <iostream>

using namespace std;

class Log {
public:
    static void Info(const char* message);
    static void Info(const char* message, char* param);
    static void Info(const char* message, unsigned short param);
};

void Log::Info(const char* message) {
    SYSTEMTIME time;
    GetLocalTime(&time);
    cout << "[" << time.wYear << "/" << time.wMonth << "/" << time.wDay << " " << time.wHour << ":" << time.wMinute << ":" << time.wSecond << "] " << message << endl;
}

void Log::Info(const char* message, char* param) {
    SYSTEMTIME time;
    GetLocalTime(&time);
    cout << "[" << time.wYear << "/" << time.wMonth << "/" << time.wDay << " " << time.wHour << ":" << time.wMinute << ":" << time.wSecond << "] " << message << param << endl;
}

void Log::Info(const char* message, unsigned short param) {
    SYSTEMTIME time;
    GetLocalTime(&time);
    cout << "[" << time.wYear << "/" << time.wMonth << "/" << time.wDay << " " << time.wHour << ":" << time.wMinute << ":" << time.wSecond << "] " << message << param << endl;
}