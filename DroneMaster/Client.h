#pragma once
#include "stdafx.h"
#include "NetworkClient.h"
class Client
{
public:
	Client();
private:
	void loop();
	CommandPackage* commandPack = new CommandPackage;
	StreamPackage* streamPack = new StreamPackage;
	NetworkClient client;
	ULONG64 tick = 0;
	cv::Mat frame, fliped;
	
};

