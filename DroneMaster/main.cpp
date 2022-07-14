#include"stdafx.h"
#include"Server.h"
#include"Client.h"

int main(int argc, char* argv[])
{
	std::cout << "Working in Mode: " << argv[1] << std::endl;
	if (strcmp(argv[1], "srv") == 0)
	{
		Server server;
		return 0;
	}
	else if (strcmp(argv[1], "cli") == 0)
	{
		Client client;
		return 0;
		//CommandPackage* commandPack = new CommandPackage;
		//
		//NetworkClient client;
		//int counter = 10000;

		//StreamPackage* streamPack = new StreamPackage;


		//cv::Mat frame, fliped;

		//while (counter != 0)
		//{
		//	counter--;
		//	client.recvDataPackage(*commandPack);
		//	commandPack->timestamp++;
		//	client.sendDataPackage(*commandPack);

		//	streamPackageClear(*streamPack);
		//	client.recvDataPackage(*streamPack);
		//	std::vector<BYTE> streamBuffer(streamPack->data, streamPack->data + streamPack->dataSize);
		//	frame = cv::imdecode(streamBuffer, cv::IMREAD_COLOR);
		//	if (!frame.empty())
		//	{
		//		cv::flip(frame, fliped, 1);
		//		cv::imshow("Video", fliped);
		//	}
		//	else
		//	{
		//		std::cerr << "Empty FRAME" << std::endl;
		//	}
		//	if (cv::waitKey(30) >= 0) break;
		//}
	}
	return 0;
}
