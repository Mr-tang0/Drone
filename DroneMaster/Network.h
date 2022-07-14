#pragma once
#include"stdafx.h"
#include"DataManager.h"

constexpr USHORT PORT = 62627;
constexpr UINT BUFFER_SIZE = 40960;
class Network
{
public:
	Network();
	int initBase();
	SOCKET socketID;

};

