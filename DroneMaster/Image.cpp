#include"Image.h"

void imageJob()
{
	cv::VideoCapture camera;
	camera.open(0);
	if (!camera.isOpened())
	{
		std::cout << "fail to open camera" << std::endl;
	}
	cv::Mat frame;
	cv::Mat flipped;
	std::vector<BYTE> buffer;

	std::vector<int> encodePara;
	encodePara.push_back(cv::IMWRITE_JPEG_QUALITY);
	encodePara.push_back(60);
	while (true)
	{
		camera >> frame;
		if (frame.empty())
		{
			std::cout << "frame is empty" << std::endl;
		}
		else 
		{
			cv::Size frameSize = cv::Size(640, 480);
			cv::resize(frame, flipped, frameSize);
			cv::imencode(".jpg", flipped, buffer, encodePara);
			std::cout << buffer.size() << std::endl;
			cv::Mat frameDecoded = cv::imdecode(buffer, cv::IMREAD_COLOR);

			cv::flip(frameDecoded, flipped, 1);
			cv::imshow("Video", flipped);
		}
		if (cv::waitKey(30) >= 0) break;
	}
	camera.release();
	cv::destroyWindow("Video");
}
