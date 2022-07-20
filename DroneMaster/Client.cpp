#include "Client.h"

Client::Client()
{
	//SDL init
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
		std::cerr << "SDL init failed" << std::endl;
	if (IMG_Init(IMG_INIT_JPG) == 0)
		std::cerr << "SDL Image init failed" << std::endl;
	if (TTF_Init() == -1)
		std::cerr << "SDL TTF init failed" << std::endl;
	
	// Create a window
	windowMain = SDL_CreateWindow("Drone", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_ALLOW_HIGHDPI);
	if (windowMain == NULL) std::cerr << "SDL window create failed" << std::endl;
	
	// Load image as surface
	imgSurface = IMG_Load("test.jpg");
	if (imgSurface == NULL)
		std::cerr << IMG_GetError() << std::endl;
	windowSurface = SDL_GetWindowSurface(windowMain);
	optimizedSurface = SDL_ConvertSurface(imgSurface, windowSurface->format, 0);
	SDL_FreeSurface(imgSurface);

	// Setup UI font
	uiFont = TTF_OpenFont("tahoma.ttf", 40);
	if (!uiFont)
		std::cerr << TTF_GetError() << std::endl;
	
	loop();
}

void Client::loop()
{
	while (go) { // debug


		client.sendDataPackage(*commandPack);
		
		// exchange data here

		client.recvDataPackage(*commandPack);
		client.sendDataPackage(*commandPack);

		streamPackageClear(*streamPack);
		client.recvDataPackage(*streamPack);
		std::vector<BYTE> streamBuffer(streamPack->data, streamPack->data + streamPack->dataSize);
		frame = cv::imdecode(streamBuffer, cv::IMREAD_COLOR);
		if (!frame.empty())
		{
			cv::flip(frame, fliped, 1);
			// Convert cv::Mat to SDL_Surface
			frameSurface = SDL_CreateRGBSurfaceFrom((void*)frame.data, frame.size().width, frame.size().height, 24, frame.size().width * 3, 0xff0000, 0x00ff00, 0x0000ff, 0);
			// Blit(copy) frameSurface to windowSurface
			SDL_UpperBlitScaled(frameSurface, NULL, windowSurface, &frameRect);
			writeLine("CAM", 0, 0);
			SDL_UpdateWindowSurface(windowMain);
		}
		else
		{
			std::cerr << "empty frame" << std::endl;
		}
		if (cv::waitKey(30) >= 0) break;
	}
}

void Client::eventSolve()
{

}

void Client::writeLine(char* text, int x, int y)
{
	SDL_Rect rect = { x, y };
	const char textConst = *text;
	textSurface = TTF_RenderUTF8_Blended(uiFont, &textConst, white);
	rect.w = textSurface->w;
	rect.h = textSurface->h;
	SDL_UpperBlitScaled(textSurface, NULL, windowSurface, &rect);
	SDL_FreeSurface(textSurface);
}

void Client::writeLine(std::string text, int x, int y)
{
	SDL_Rect rect = { x, y };
	textSurface = TTF_RenderUTF8_Blended(uiFont, text.c_str(), white);
	rect.w = textSurface->w;
	rect.h = textSurface->h;
	SDL_UpperBlitScaled(textSurface, NULL, windowSurface, &rect);
	SDL_FreeSurface(textSurface);
}

void Client::cleanUp()
{
	SDL_FreeSurface(frameSurface);
}

