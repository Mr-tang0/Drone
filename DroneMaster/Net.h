#pragma once
#include "stdafx.h"

constexpr auto PORT_DEFAULT = 23456;
constexpr auto PACKAGE_SIZE = 4096;
constexpr auto PACKAGE_MAXSIZE = 0xFFFF;

class NetBase
{
public:
	NetBase();
	~NetBase();
    SOCKET getSocket();

private:
	int initBase();
    SOCKET socket_ = INVALID_SOCKET;
};

class NetServer
{
public:
    NetServer();
    ~NetServer();
    uint packageSend(uchar* buffer, uint size);
    uchar* packageRecv(uint& size);
    int init();

private:
    NetBase* base;
    SOCKET sock = INVALID_SOCKET;
};

class NetClient
{
public:
    NetClient();
    ~NetClient();
    uint packageSend(uchar* buffer, uint size);
    uchar* packageRecv(uint& size);
    int setServerIP(char* ip);
    int init();

private:
    NetBase* base;
    SOCKET sock = INVALID_SOCKET;
    char serverIP[16] = "127.0.0.1";
};
