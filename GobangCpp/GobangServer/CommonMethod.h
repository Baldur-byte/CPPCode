#pragma once
#include <iostream>
#include <WinSock2.h>

using namespace std;

class CommonMethod
{
public:
    static char* AddrToStr(SOCKADDR_IN addr){
        wchar_t ipstr[32];
        size_t len = 32;
        WSAAddressToString((SOCKADDR*)&addr, sizeof(struct sockaddr_in), 0, ipstr, (LPDWORD) & len);
        int size = WideCharToMultiByte(CP_ACP, 0, ipstr, -1, NULL, 0, NULL, NULL);
        char* ip = new char[size];
        WideCharToMultiByte(CP_ACP, 0, ipstr, -1, ip, size, NULL, NULL);
        return ip;
    }
};