#pragma once
#include "stdafx.h"
#include "DataManager.h"
#include "Net.h"
class Server
{
public:
	Server();
private:
	bool go = true;
	void loop();
	CommandPackage* commandPack = new CommandPackage;
	NetServer netServer;
	cv::VideoCapture camera;
	cv::Mat frame;
	std::vector<uchar> buffer;
	std::vector<int> encodePara;
};

