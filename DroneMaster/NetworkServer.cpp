#include "NetworkServer.h"

NetworkServer::NetworkServer()
{
    init();
}

int NetworkServer::init()
{
    SOCKADDR_IN addrSrv{AF_INET, PORT, INADDR_ANY};
    
    int result = bind(socketID, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
    if (result == -1)
    {
        std::cerr << "Socket bind Failed " << WSAGetLastError() << std::endl;
        closesocket(socketID);
        WSACleanup();
    }
    
    result = listen(socketID, SOMAXCONN);
    if (result == -1)
    {
        std::cerr << "Socket listen Failed " << WSAGetLastError() << std::endl;
        closesocket(socketID);
        WSACleanup();
    }
    std::cout << "Network server init succeed" << std::endl;
    std::cout << "Waiting for Connection..." << std::endl;
    SOCKADDR_IN addrCli;
    socklen_t addrCliSize = sizeof(addrCli);
    SOCKET socket2Client = accept(socketID, (SOCKADDR*)&addrCli, &addrCliSize);
    if (socket2Client == INVALID_SOCKET)
        std::cerr << "Socket to Client build Failed " << WSAGetLastError() << std::endl;
    socketServer = socket2Client;
    std::cout << "Connected" << std::endl;
    char ip[16];
    inet_ntop(AF_INET, (void*)&addrCli.sin_addr, ip, 16);
    std::cout << "IP: " << ip << "  Port: " << PORT << std::endl;
    return 0;
}

int NetworkServer::sendPack(int dataSize)
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
        memmove(sendBuffer+6, data, dataSize*sizeof(char));
        int result = send(socketServer, (char*)sendBuffer, dataSize+6, 0);
        if (result == -1)
            std::cerr << "Server Send Failed " << WSAGetLastError() << std::endl;
    }
    else
        std::cerr << "Huge Pack! PackSize: " << dataSize + 6 << std::endl;
    return 0;
}

int NetworkServer::recvPack()
{
    memset(recvBuffer, 0, sizeof(sendBuffer));
    int recvSize = recv(socketServer, (char*)recvBuffer, 6, 0);
    if (recvSize > 0)
    {
        if (recvBuffer[0] == 0xEF && recvBuffer[1] == 0xEF && recvBuffer[2] == 0xEF && recvBuffer[3] == 0xEF)
        {
            int dataSize = recvBuffer[4] * 256 + recvBuffer[5];
            memset(recvBuffer, 0, sizeof(char) * 6);
            assert(recv(socketServer, (char*)recvBuffer, dataSize, 0) == dataSize);
            memmove(data, recvBuffer, dataSize);
            return dataSize;
        }
    }
    return 0;
}

int NetworkServer::sendDataPackage(CommandPackage &dataPack)
{
    memset(data, 0, sizeof(data));
    memmove(data, (void*)&dataPack, sizeof(CommandPackage));
    sendPack(sizeof(CommandPackage));
    return 0;
}

int NetworkServer::recvDataPackage(CommandPackage& dataPack)
{
    recvPack();
    memmove((void*)&dataPack, data, sizeof(dataPack));
    return 0;
}

int NetworkServer::sendDataPackage(StreamPackage& dataPack)
{
    memset(data, 0, sizeof(data));
    memmove(data, (void*)&dataPack.data, sizeof(BYTE) * dataPack.dataSize);
    sendPack(sizeof(BYTE) * dataPack.dataSize);
    return 0;
}

int NetworkServer::recvDataPackage(StreamPackage& dataPack)
{
    int packSize = recvPack();
    std::cout << "\b\b\b\b\b";
    if (packSize < 10000)
        std::cout << '0' << packSize;
    else std::cout << packSize;
    dataPack.dataSize = packSize;
    memmove((void*)&dataPack.data, data, packSize);
    return 0;
}
