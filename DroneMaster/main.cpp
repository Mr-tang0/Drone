#include"stdafx.h"
#include"NetworkServer.h"
#include"NetworkClient.h"
#include"DataManager.h"
#include"Image.h"
int main(int argc, char* argv[])
{
	if (strcmp(argv[1], "srv") == 0)
	{
		CommandPackage* commandPack = new CommandPackage;

		NetworkServer server;
		int counter = 10000;
		
		commandPack->timestamp = 0;

		StreamPackage* streamPack = new StreamPackage;
		cv::VideoCapture camera(0);
		if (!camera.isOpened())
		{
			std::cout << "fail to open camera" << std::endl;
		}
		cv::Mat frame;

		std::vector<int> encodePara;
		encodePara.push_back(cv::IMWRITE_JPEG_QUALITY);
		encodePara.push_back(60);

		std::vector<BYTE> buffer;
		buffer.resize(BUFFER_SIZE);

		while (counter != 0)
		{
			counter--;
			server.sendDataPackage(*commandPack);
			server.recvDataPackage(*commandPack);

			camera >> frame;
			if (frame.empty())
			{
				std::cout << "frame is empty" << std::endl;
			}
			else
			{
				cv::imencode(".jpg", frame, buffer, encodePara);
				if (!buffer.empty())
				{
					streamPackageClear(*streamPack);
					streamPack->dataSize = buffer.size();
					memcpy(streamPack->data, &buffer[0], buffer.size() * sizeof(BYTE));
				}
				else
				{
					std::cout << "Empty Buffer" << std::endl;
				}
			}
			server.sendDataPackage(*streamPack);
		}
	}
	else if (strcmp(argv[1], "cli") == 0)
	{
		CommandPackage* commandPack = new CommandPackage;
		
		NetworkClient client;
		int counter = 10000;

		StreamPackage* streamPack = new StreamPackage;


		cv::Mat frame;

		while (counter != 0)
		{
			counter--;
			client.recvDataPackage(*commandPack);
			commandPack->timestamp++;
			client.sendDataPackage(*commandPack);

			streamPackageClear(*streamPack);
			client.recvDataPackage(*streamPack);
			std::vector<BYTE> streamBuffer(streamPack->data, streamPack->data + streamPack->dataSize);
			frame = cv::imdecode(streamBuffer, cv::IMREAD_COLOR);
			if (!frame.empty())
			{
				cv::imshow("Video", frame);
			}
			else
			{
				std::cout << "Empty FRAME" << std::endl;
			}
			if (cv::waitKey(30) >= 0) break;
		}
	}
	return 0;
}