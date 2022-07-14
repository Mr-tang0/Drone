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
        std::cout << "Socket bind Failed " << WSAGetLastError() << std::endl;
        closesocket(socketID);
        WSACleanup();
    }
    
    result = listen(socketID, SOMAXCONN);
    if (result == -1)
    {
        std::cout << "Socket listen Failed " << WSAGetLastError() << std::endl;
        closesocket(socketID);
        WSACleanup();
    }
    std::cout << "Network server init succeed" << std::endl;
    std::cout << "Waiting for Connection..." << std::endl;
    SOCKADDR_IN addrCli;
    socklen_t addrCliSize = sizeof(addrCli);
    SOCKET socket2Client = accept(socketID, (SOCKADDR*)&addrCli, &addrCliSize);
    if (socket2Client == INVALID_SOCKET)
    {
        std::cout << "Socket to Client build Failed " << WSAGetLastError() << std::endl;
    }
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
        memcpy(sendBuffer+6, data, dataSize*sizeof(char));
        int result = send(socketServer, (char*)sendBuffer, dataSize+6, 0);
        if (result == -1)
        {
            std::cout << "Server Send Failed " << WSAGetLastError() << std::endl;
        }
    }
    else
    {
        std::cout << "Huge Pack! PackSize: " << dataSize + 6 << std::endl;
    }
    return 0;
}

int NetworkServer::recvPack()
{
    
    memset(recvBuffer, 0, sizeof(sendBuffer));
    int recvSize = recv(socketServer, (char*)recvBuffer, BUFFER_SIZE, 0);
    if (recvSize > 0)
    {
        if (recvBuffer[0] == 0xEF && recvBuffer[1] == 0xEF && recvBuffer[2] == 0xEF && recvBuffer[3] == 0xEF)
        {
            int dataSize = recvBuffer[4] * 256 + recvBuffer[5];
            memcpy(data, recvBuffer + 6, dataSize);
            this->dataSize = dataSize;
        }
    }
    return 0;
}

int NetworkServer::sendDataPackage(CommandPackage &dataPack)
{
    memset(data, 0, sizeof(data));
    memcpy(data, (void*)&dataPack, sizeof(CommandPackage));
    sendPack(sizeof(CommandPackage));
    return 0;
}

int NetworkServer::recvDataPackage(CommandPackage& dataPack)
{
    recvPack();
    memcpy((void*)&dataPack, data, sizeof(dataPack));
    return 0;
}

int NetworkServer::sendDataPackage(StreamPackage& dataPack)
{
    memset(data, 0, sizeof(data));
    memcpy(data, (void*)&dataPack, sizeof(StreamPackage));
    sendPack(sizeof(StreamPackage));
    return 0;
}

int NetworkServer::recvDataPackage(StreamPackage& dataPack)
{
    recvPack();
    memcpy((void*)&dataPack, data, sizeof(dataPack));
    return 0;
}
