#include "NetworkClient.h"

NetworkClient::NetworkClient()
{
    init();
}

int NetworkClient::init()
{
    SOCKADDR_IN addrSrv{AF_INET, PORT};
    inet_pton(AF_INET, "127.0.0.1", &(addrSrv.sin_addr));
    std::cout << "Connecting to Server..." << std::endl;
    int result = connect(socketID, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
    if (result == -1)
    {
        int errorID = WSAGetLastError();
        if (errorID == 10061)
        {
            std::cerr << "Socket connect Failed " << "Ô¶³Ì»ý¼«¾Ü¾ø Retry" << std::endl;
            init();
            return 0;
        }
        else 
            std::cerr << "Socket connect Failed ID:" << errorID << std::endl;
    }
    socketClient = socketID;
    std::cout << "Connected" << std::endl;
    char ip[16];
    inet_ntop(AF_INET, (void*)&addrSrv.sin_addr, ip, 16);
    std::cout << "IP: " << ip << "  Port: " << PORT << std::endl;
    return 0;
}

int NetworkClient::sendPack(int dataSize)
{
    memset(sendBuffer, 0, sizeof(sendBuffer));
    if (dataSize <= BUFFER_SIZE - 6)
    {
        sendBuffer[0] = 0xEF;
        sendBuffer[1] = 0xEF;
        sendBuffer[2] = 0xEF;
        sendBuffer[3] = 0xEF;
        sendBuffer[4] = dataSize / 256;
        sendBuffer[5] = dataSize % 256;
        memmove(sendBuffer + 6, data, dataSize * sizeof(char));
        int result = send(socketClient, (char*)sendBuffer, dataSize + 6, 0);
        if (result == -1)
        {
            std::cerr << "Server Send Failed " << WSAGetLastError() << std::endl;
        }
    }
    else
    {
        std::cerr << "Huge Pack! PackSize: " << dataSize + 6 << std::endl;
    }
    return 0;
}

int NetworkClient::recvPack()
{
    memset(recvBuffer, 0, sizeof(sendBuffer));
    int recvSize = recv(socketClient, (char*)recvBuffer, 6, 0);
    if (recvSize > 0)
    {
        if (recvBuffer[0] == 0xEF && recvBuffer[1] == 0xEF && recvBuffer[2] == 0xEF && recvBuffer[3] == 0xEF)
        {
            int dataSize = recvBuffer[4] * 256 + recvBuffer[5];
            memset(recvBuffer, 0, sizeof(char) * 6);
            assert(recv(socketClient, (char*)recvBuffer, dataSize, 0) == dataSize);
            memmove(data, recvBuffer, dataSize);
            return dataSize;
        }
    }
    return 0;
}

int NetworkClient::sendDataPackage(CommandPackage& dataPack)
{
    memset(data, 0, sizeof(data));
    memmove(data, (void*)&dataPack, sizeof(CommandPackage));
    sendPack(sizeof(CommandPackage));
    return 0;
}

int NetworkClient::recvDataPackage(CommandPackage& dataPack)
{
    recvPack();
    memmove((void*)&dataPack, data, sizeof(dataPack));
    return 0;
}

int NetworkClient::sendDataPackage(StreamPackage& dataPack)
{
    memset(data, 0, sizeof(data));
    memmove(data, (void*)&dataPack.data, sizeof(BYTE) * dataPack.dataSize);
    sendPack(sizeof(BYTE) * dataPack.dataSize);
    return 0;
}

int NetworkClient::recvDataPackage(StreamPackage& dataPack)
{
    int packSize = recvPack();
    std::cout << "\b\b\b\b\b";
    if (packSize < 10000) {
        std::cout << '0' << packSize;
    }
    else std::cout << packSize;
    dataPack.dataSize = packSize;
    memmove((void*)&dataPack.data, data, packSize);
    return 0;
}
