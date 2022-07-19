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

		//if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
		//	std::cerr << "SDL init failed" << std::endl;
		//if(IMG_Init(IMG_INIT_JPG) == 0)
		//	std::cerr << "SDL Image init failed" << std::endl;
		//if(TTF_Init() == -1)
		//	std::cerr << "SDL TTF init failed" << std::endl;

		//SDL_Window* windowMain = nullptr;
		//windowMain = SDL_CreateWindow("Drone", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_ALLOW_HIGHDPI);
		//if (windowMain == NULL) std::cerr << "SDL window create failed" << std::endl;
		//
		//SDL_Surface* img = IMG_Load("test.jpg");
		//if (img == NULL)
		//	std::cerr << IMG_GetError() << std::endl;


		//SDL_Surface* windowSurface = SDL_GetWindowSurface(windowMain);
		//SDL_Surface* optimizedSurface = SDL_ConvertSurface(img, windowSurface->format, 0);
		//SDL_FreeSurface(img);
		//TTF_Font* uiFont = TTF_OpenFont("tahoma.ttf", 32);
		//if (!uiFont)
		//	std::cerr << TTF_GetError() << std::endl;
		//SDL_Color white = { 255, 255, 255 };
		//SDL_Color black = { 0, 0, 0 };
		//SDL_Surface* textSurface = TTF_RenderUTF8_Blended(uiFont, "What Ever", white);

		//cv::VideoCapture camera(0);
		//if (!camera.isOpened())
		//{
		//	std::cerr << "fail to open camera" << std::endl;
		//}
		//cv::Mat frame, fliped;

		//while (true)
		//{
		//	camera >> frame;
		//	cv::flip(frame, fliped, 1);

		//	SDL_Surface* frameSurface = SDL_CreateRGBSurfaceFrom((void*)frame.data, frame.size().width, frame.size().height, 24, frame.size().width*3, 0xff0000, 0x00ff00, 0x0000ff, 0);
		//	SDL_BlitSurface(frameSurface, NULL, windowSurface, NULL);
		//	SDL_BlitSurface(textSurface, NULL, windowSurface, NULL);
		//	SDL_FreeSurface(frameSurface);
		//	SDL_UpdateWindowSurface(windowMain);
		//}
		return 0;
	}
	std::cout << "Run in command-line mode with \"DroneMaster(.exe) srv\" for server side or \"DroneMaster(.exe) cli\" for client side" << std::endl;
	return 0;
}
