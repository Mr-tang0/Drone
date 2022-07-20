#pragma once
#include "stdafx.h"
#include "NetworkServer.h"
class Server
{
public:
	Server();
private:
	bool go = true;
	void loop();
	CommandPackage* commandPack = new CommandPackage;
	StreamPackage* streamPack = new StreamPackage;
	NetworkServer server;
	cv::VideoCapture camera;
	cv::Mat frame;
	std::vector<BYTE> buffer;
	std::vector<int> encodePara;
};

