#include "Server.h"

Server::Server()
{
	// init opencv camera
	camera.open(0);
	if (!camera.isOpened())
	{
		std::cerr << "fail to open camera" << std::endl;
	}
	encodePara.push_back(cv::IMWRITE_JPEG_QUALITY);
	encodePara.push_back(60);

	// init buffer
	buffer.resize(BUFFER_SIZE);

	// start loop
	loop();
}

void Server::loop()
{
	while(this->tick < 102400){ // debug
		this->tick++;

		// exchange data here

		server.sendDataPackage(*commandPack);
		server.recvDataPackage(*commandPack);

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
				std::cerr << "Empty Buffer" << std::endl;
			}
		}
		server.sendDataPackage(*streamPack);
	}
}
