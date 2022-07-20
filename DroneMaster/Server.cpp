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
	encodePara.push_back(65);

	// init buffer
	buffer.resize(BUFFER_SIZE);

	// Setup network
	
	netServer.init();

	// start loop
	loop();
}

void Server::loop()
{
	while(go){

		// exchange data here

		netServer.recvDataPackage(*commandPack);
		netServer.sendDataPackage(*commandPack);

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
				streamPackageClear(*streamPack);
				streamPack->dataSize = buffer.size();
				memmove(streamPack->data, &buffer[0], buffer.size() * sizeof(BYTE));
			}
			else
			{
				std::cerr << "buffer empty" << std::endl;
			}
		}
		netServer.sendDataPackage(*streamPack);
	}
}
