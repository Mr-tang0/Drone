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

inline NetClient::NetClient()
{
    base = new NetBase();
}


inline NetClient::~NetClient()
{
    delete base;
}

inline uint NetClient::packageSend(uchar* buffer, uint size)
{
    if (size > PACKAGE_MAXSIZE)
        return 0;
    uchar* out = new uchar[size + 10];
    memset(out, 0, static_cast<size_t>(size) + 10);
    memset(out, 0xef, 4);
    out[4] = size & 0x00ff;
    out[5] = size >> 8;

    memmove(out + 6, buffer, size);
    memset(out + 6 + size, 0xfe, 4);
    uint sendBytes = 0;
    uint sendingSize;
    size += 10;
    while (sendBytes < size)
    {
        if ((size - sendBytes) < PACKAGE_SIZE)
            sendingSize = size - sendBytes;
        else
            sendingSize = PACKAGE_SIZE;
#ifdef _DEBUG
        std::cout << "Sending Bytes: " << sendingSize << std::endl;
#endif // _DEBUG
        sendBytes += send(sock, (char*)out + sendBytes, sendingSize, 0);
    }
    delete[] out;
    return sendBytes;
}

inline uchar* NetClient::packageRecv(uint& size)
{
    uchar header[6] = { 0 };
    int recvingBytes = recv(sock, (char*)header, 6, 0);
    if (recvingBytes != 6)
    {
        printf("%x %x %x %x RECV HEAD FAILED %d ", header[0], header[1], header[2], header[3], recvingBytes);
        if (recvingBytes == -1)
            std::cout << WSAGetLastError() << std::endl;
        throw;
    }
    if ((header[0] != 0xef) || (header[3] != 0xef))
    {
        printf("%x %x %x %x HEADER ERROR", header[0], header[1], header[2], header[3]);
        throw;
    }
    uint recvSize = header[4] | (header[5] << 8);
    #ifdef _DEBUG
    std::cout << "Try Recving Bytes: " << recvSize << std::endl;
    #endif // _DEBUG
    uchar* in = new uchar[recvSize];
    size = 0;
    while (size < recvSize)
    {
        size += recv(sock, (char*)in, recvSize - size, 0);
    }
    recvingBytes = recv(sock, (char*)header, 4, 0);
    assert(recvingBytes == 4);
    if ((header[0] != 0xfe) || (header[3] != 0xfe))
    {
        printf("%x %x %x %x TAIL ERROR", header[0], header[1], header[2], header[3]);
        throw;
    }
    return in;
}

inline int NetClient::setServerIP(char* ip)
{
    strcpy_s(serverIP, ip);
    return 0;
}

inline int NetClient::init()
{
    SOCKADDR_IN addrCli{};
    addrCli.sin_family = AF_INET;
    addrCli.sin_port = htons(PORT_DEFAULT);
    inet_pton(AF_INET, serverIP, &(addrCli.sin_addr));
    std::cout << "Connecting to Server..." << std::endl;
    int result = connect(base->getSocket(), (SOCKADDR*)&addrCli, sizeof(SOCKADDR));
    if (result == INVALID_SOCKET)
    {
        int errorID = WSAGetLastError();
        if (errorID == 10061)
        {
            std::cerr << "Socket connect Failed " << "Ô¶³Ì»ý¼«¾Ü¾ø Retry" << std::endl;
            Sleep(1000);
            return init();
        }
        else 
        {
            std::cerr << "Socket connect Failed ERROR ID:" << errorID << std::endl;
            exit(-1);
        }
    }
    sock = base->getSocket();
    std::cout << "Connected" << std::endl;
    char ip[16];
    inet_ntop(AF_INET, (void*)&addrCli.sin_addr, ip, 16);
    std::cout << "IP: " << ip << "  Port: " << PORT_DEFAULT << std::endl;
    return 0;
}

inline NetServer::NetServer()
{
    base = new NetBase();
}

inline NetServer::~NetServer()
{
    if (sock != INVALID_SOCKET)
        closesocket(sock);
    delete base;
}

inline uint NetServer::packageSend(uchar* buffer, uint size)
{
    if (size > PACKAGE_MAXSIZE)
        return 0;
    uchar* out = new uchar[size + 10];
    memset(out, 0, static_cast<size_t>(size) + 10);
    memset(out, 0xef, 4);
    out[4] = size & 0x00ff;
    out[5] = size >> 8;
    memmove(out + 6, buffer, size);
    memset(out + 6 + size, 0xfe, 4);
    uint sendBytes = 0;
    uint sendingSize;
    size += 10;
    while(sendBytes < size)
    {
        if ((size - sendBytes) < PACKAGE_SIZE)
            sendingSize = size - sendBytes;
        else
            sendingSize = PACKAGE_SIZE;
#ifdef _DEBUG
        std::cout << "Sending Bytes: " << sendingSize << std::endl;
#endif // _DEBUG
        sendBytes += send(sock, (char*)out + sendBytes, sendingSize, 0);

    }
    delete [] out;
    return sendBytes;
}

inline uchar* NetServer::packageRecv(uint& size)
{
    uchar header[6] = { 0 };
    int recvingBytes = recv(sock, (char*)&header, 6, 0);
    if (recvingBytes != 6)
    {
        printf("%x %x %x %x RECV HEAD FAILED %d", header[0], header[1], header[2], header[3], recvingBytes);
        throw;
    }
    if (!(header[0] == 0xef))
    {
        printf("%x %x %x %x HEADER ERROR", header[0], header[1], header[2], header[3]);
        throw;
    }
    uint recvSize = header[4] | ( header[5] << 8);
    #ifdef _DEBUG
    std::cout << "Try Recving Bytes: " << recvSize << std::endl;
    #endif // _DEBUG
    uchar* in = new uchar[recvSize];
    size = 0;
    while (size < recvSize)
    {
        size += recv(sock, (char*)in, recvSize - size, 0);
    }
    recvingBytes = recv(sock, (char*)header, 4, 0);
    assert(recvingBytes == 4);
    if (!(header[0] == 0xfe))
    {
        printf("%x %x %x %x TAIL ERROR", header[0], header[1], header[2], header[3]);
        throw;
    }
    return in;
}

inline int NetServer::init()
{
    SOCKADDR_IN addrSrv{};
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_addr.S_un.S_addr = INADDR_ANY;
    addrSrv.sin_port = htons(PORT_DEFAULT);

    int result = bind(base->getSocket(), (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
    if (result == SOCKET_ERROR)
    {
        std::cerr << "Socket bind Failed " << WSAGetLastError() << std::endl;
        delete base;
        Sleep(1000);
        base = new NetBase();
        return init();
    }
    result = listen(base->getSocket(), 5);
    if (result == SOCKET_ERROR)
    {
        std::cerr << "Socket listen Failed " << WSAGetLastError() << std::endl;
        delete base;
        Sleep(1000);
        base = new NetBase();
        return init();
    }
    std::cout << "Waiting for Connection..." << std::endl;
    SOCKADDR_IN addrCli{};
    socklen_t addrCliSize = sizeof(addrCli);
    SOCKET socket2Client = accept(base->getSocket(), (SOCKADDR*)&addrCli, &addrCliSize);
    if (socket2Client == INVALID_SOCKET)
    {
        std::cerr << "Socket to Client build Failed " << WSAGetLastError() << std::endl;
        delete base;
        Sleep(1000);
        base = new NetBase();
        return init();
    }
    sock = socket2Client;
    std::cout << "Connected" << std::endl;
    char ip[16];
    inet_ntop(AF_INET, (void*)&addrCli.sin_addr, ip, 16);
    std::cout << "IP: " << ip << "  Port: " << PORT_DEFAULT << std::endl;
    return 0;
}

inline NetBase::NetBase()
{
    initBase();
}

inline NetBase::~NetBase()
{
    std::cout << "BASE EXIT" << std::endl;
    if (socket_ != INVALID_SOCKET)
        closesocket(socket_);
    WSACleanup();
}

inline SOCKET NetBase::getSocket()
{
    return socket_;
}

inline int NetBase::initBase()
{
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        std::cerr << "WSAStartup Failed " << WSAGetLastError() << std::endl;
        initBase();
        return 0;
    }

    SOCKET sockResult = socket(AF_INET, SOCK_STREAM, 0);
    if (sockResult == INVALID_SOCKET)
    {
        std::cerr << "Socket Build Failed " << WSAGetLastError() << std::endl;
        Sleep(1000);
        WSACleanup();
        initBase();
        return 0;
    }
    else this->socket_ = sockResult;
    #ifdef _DEBUG
    std::cout << "Network base init succeed" << std::endl;
    #endif // _DEBUG
	return 0;
}
