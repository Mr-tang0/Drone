#include "Server.h"

Server::Server()
{
	// init opencv camera
	camera.open(0);
	camera.set(cv::CAP_PROP_FRAME_WIDTH, 480);
	camera.set(cv::CAP_PROP_FRAME_HEIGHT, 640);
	camera.set(cv::CAP_PROP_FPS, 30);
	if (!camera.isOpened())
		std::cerr << "fail to open camera" << std::endl;
	encodePara.push_back(cv::IMWRITE_JPEG_QUALITY);
	encodePara.push_back(50);

	// Setup network
	netServer.init();

	// start loop
	loop();
}

void Server::loop()
{
	while(go)
	{
		// Exchange CommandPackage
		uchar* recvBuffer;
		uint recvSize = 0;
		
		recvBuffer = netServer.packageRecv(recvSize);
		if (recvSize != sizeof(CommandPackage))
			std::cout << "CommandPack size unpair: " << recvSize << sizeof(CommandPackage) << std::endl;
		memmove(commandPack, recvBuffer, sizeof(CommandPackage));
		commandPack->go = go;

		netServer.packageSend(recvBuffer, recvSize);
		
		delete[] recvBuffer;

		// Fetch a frame from camera
		this->camera >> this->frame;
		if (this->frame.empty())
			std::cerr << "frame is empty" << std::endl;
		else
		{
			// Encode
			cv::imencode(".jpg", this->frame, this->buffer, this->encodePara);
			if (!this->buffer.empty())
			{
				// Send to Client
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
