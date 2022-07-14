#pragma once
#include "Network.h"
class NetworkServer :
    public Network
{
public:
    NetworkServer();
    int init();
    SOCKET socketServer;
    int sendPack(int dataSize);
    int recvPack();
    int sendDataPackage(CommandPackage& dataPack);
    int recvDataPackage(CommandPackage& dataPack);
    int sendDataPackage(StreamPackage& dataPack);
    int recvDataPackage(StreamPackage& dataPack);
    BYTE* recvBuffer = new BYTE[BUFFER_SIZE];
    BYTE* sendBuffer = new BYTE[BUFFER_SIZE];
    BYTE* data = new BYTE[BUFFER_SIZE];
    UINT dataSize;
};

