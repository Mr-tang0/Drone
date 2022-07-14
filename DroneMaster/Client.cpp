#include "Client.h"

Client::Client()
{
	loop();
}

void Client::loop()
{
	while (this->tick < 102400) { // debug
		this->tick++;
		client.recvDataPackage(*commandPack);
		
		// exchange data here

		client.sendDataPackage(*commandPack);
		streamPackageClear(*streamPack);
		client.recvDataPackage(*streamPack);
		std::vector<BYTE> streamBuffer(streamPack->data, streamPack->data + streamPack->dataSize);
		frame = cv::imdecode(streamBuffer, cv::IMREAD_COLOR);
		if (!frame.empty())
		{
			cv::flip(frame, fliped, 1);
			cv::imshow("Video", fliped);
		}
		else
		{
			std::cerr << "Empty FRAME" << std::endl;
		}
		if (cv::waitKey(30) >= 0) break;
	}
}
