//#include "Network.h"
//
//Network::Network()
//{
//    initBase();
//}
//
//int Network::initBase()
//{
//    WSADATA wsaData;
//    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
//    if (result != 0)
//    {
//        std::cerr << "WSAStartup Failed " << WSAGetLastError() << std::endl;
//        initBase();
//        return 0;
//    }
//
//    SOCKET sockResult = socket(AF_INET, SOCK_STREAM, 0);
//    if (sockResult == INVALID_SOCKET)
//    {
//        std::cerr << "Socket Build Failed " << WSAGetLastError() << std::endl;
//        WSACleanup();
//        initBase();
//        return 0;
//    }
//    else this->socketID = sockResult;
//    std::cout << "Network base init succeed" << std::endl;
//    return 0;
//}
