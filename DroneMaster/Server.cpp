#include "Server.h"

Server::Server()
{
	// init opencv camera
	camera.set(cv::CAP_PROP_FRAME_WIDTH, 480);
	camera.set(cv::CAP_PROP_FRAME_HEIGHT, 640);
	camera.set(cv::CAP_PROP_FPS, 30);
	camera.open(0);
	if (!camera.isOpened())
	{
		std::cerr << "fail to open camera" << std::endl;
	}
	encodePara.push_back(cv::IMWRITE_JPEG_QUALITY);
	encodePara.push_back(50);

	// init buffer
	//buffer.resize(BUFFER_SIZE);

	// Setup network
	
	netServer.init();

	// start loop
	loop();
}

void Server::loop()
{
	while(go){

		// exchange data here

		uchar* recvBuffer = nullptr;
		uint recvSize = 0;
		
		recvBuffer = netServer.packageRecv(recvSize);
		if (recvSize != sizeof(commandPack))
			std::cout << recvSize << sizeof(commandPack) << std::endl;
		memmove(commandPack, recvBuffer, sizeof(commandPack));
		
		netServer.packageSend(recvBuffer, recvSize);
		
		delete[] recvBuffer;
		recvBuffer = nullptr;



		//netServer.recvDataPackage(*commandPack);
		//netServer.sendDataPackage(*commandPack);

		this->camera >> this->frame;
		if (this->frame.empty())
		{
			std::cerr << "frame is empty" << std::endl;
		}
		else
		{
			cv::imencode(".jpg", this->frame, this->buffer, this->encodePara);
			if (!this->buffer.empty())
			{
				uint sendSize = buffer.size();
				netServer.packageSend(&buffer[0], sendSize);
			}
			else
			{
				std::cerr << "buffer empty" << std::endl;
				throw;
			}
		}
	}
}
