#include "Client.h"

Client::Client(char* serverIP)
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
	uiFontLarge = TTF_OpenFont("tahoma.ttf", 36);
	if (!uiFontLarge)
		std::cerr << TTF_GetError() << std::endl;
	uiFont = TTF_OpenFont("tahoma.ttf", 18);

	// Setup network
	SDL_FillRect(windowSurface, NULL, 0x00);
	writeLine("Connecting...", 300, 300, true);
	SDL_UpdateWindowSurface(windowMain);

	netClient.serverIP = serverIP;
	netClient.init();


	loop();
}

void Client::loop()
{
	while (go) { // debug
		loopTimeA = SDL_GetTicks64();
		eventSolve();


		commandPack->timestamp = SDL_GetTicks64();
		netClient.sendDataPackage(*commandPack);
		
		// exchange data here

		netClient.recvDataPackage(*commandPack);
		recvTime = SDL_GetTicks64();
		// Video stream data pack exchange
		streamPackageClear(*streamPack);
		netClient.recvDataPackage(*streamPack);

		// decode
		std::vector<BYTE> streamBuffer(streamPack->data, streamPack->data + streamPack->dataSize);
		frame = cv::imdecode(streamBuffer, cv::IMREAD_COLOR);
		
		// show
		SDL_FillRect(windowSurface, NULL, 0x00);
		if (!frame.empty())
		{
			cv::flip(frame, fliped, 1);
			// Convert cv::Mat to SDL_Surface
			frameSurface = SDL_CreateRGBSurfaceFrom((void*)fliped.data, fliped.size().width, fliped.size().height, 24, fliped.size().width * 3, 0xff0000, 0x00ff00, 0x0000ff, 0);
			// Blit(Copy) frameSurface to windowSurface
			SDL_UpperBlitScaled(frameSurface, NULL, windowSurface, &frameRect);
			
			
			char line[64];
			sprintf_s(line, "CAM");
			writeLine(line, 0, 0, true);
			sprintf_s(line, "Forward - %d", commandPack->forward);
			writeLine(line, 645, 0);
			sprintf_s(line, "Slipping - %d", commandPack->slipping);
			writeLine(line, 645, 30);
			sprintf_s(line, "Heading - %d", commandPack->heading);
			writeLine(line, 645, 60);
			sprintf_s(line, "PING - %I64u", pingSolve());
			writeLine(line, 645, 90);
			sprintf_s(line, "Frame - %.2f", frameSolve());
			writeLine(line, 645, 120);
			sprintf_s(line, "Speed - %.2f kbps", speedSolve());
			writeLine(line, 645, 150);
		}
		else
		{
			std::cerr << "empty frame" << std::endl;
		}
		SDL_UpdateWindowSurface(windowMain);
		loopTimeB = SDL_GetTicks64() - loopTimeA;
	}
}

UINT64 Client::pingSolve()
{
	if (pingList.size() == PINGSIZE)
		pingList.pop_back();
	pingList.push_front(recvTime - commandPack->timestamp);
	UINT64 ret = 0;
	for (uint64Iter = pingList.begin(); uint64Iter != pingList.end(); uint64Iter++)
		ret += *uint64Iter;
	return ret / pingList.size();
}

float Client::frameSolve()
{
	if (loopTimeB == 0)
		return -1;
	if (frameList.size() == FRAMESIZE)
		frameList.pop_back();
	frameList.push_front(loopTimeB);
	float ret = 0;
	for (floatIter = frameList.begin(); floatIter != frameList.end(); floatIter++)
		ret += *floatIter;
	frameRate = 1000 / (ret / frameList.size());
	return frameRate;
}

float Client::speedSolve()
{
	if (frameRate == 0)
		return -1;
	if (packSizeList.size() == FRAMESIZE)
		packSizeList.pop_back();
	packSizeList.push_front(streamPack->dataSize);
	float ret = 0;
	for (floatIter = packSizeList.begin(); floatIter != packSizeList.end(); floatIter++)
		ret += *floatIter;
	return ret / packSizeList.size() * frameRate / 1024.;
}

void Client::eventSolve()
{
	while(SDL_PollEvent(&event_))
	{
		switch (event_.type)
		{
		case SDL_QUIT:
			go = false;
			break;
		case SDL_KEYDOWN:
			switch (event_.key.keysym.sym)
			{
			case SDLK_w:
				commandPack->forward = 64;
				break;
			case SDLK_s:
				commandPack->forward = -64;
				break;
			case SDLK_a:
				commandPack->slipping = 64;
				break;
			case SDLK_d:
				commandPack->slipping = -64;
				break;
			case SDLK_q:
				commandPack->heading = 64;
				break;
			case SDLK_e:
				commandPack->heading = -64;
			}
			break;
		case SDL_KEYUP:
			switch (event_.key.keysym.sym)
			{
			case SDLK_w:
				commandPack->forward = 0;
				break;
			case SDLK_s:
				commandPack->forward = 0;
				break;
			case SDLK_a:
				commandPack->slipping = 0;
				break;
			case SDLK_d:
				commandPack->slipping = 0;
				break;
			case SDLK_q:
				commandPack->heading = 0;
				break;
			case SDLK_e:
				commandPack->heading = 0;
			}
			break;
		}
	}
}

inline void Client::writeLine(char* text, int x, int y, bool large)
{
	TTF_Font* font = nullptr;
	if (large)
		font = uiFontLarge;
	else
		font = uiFont;
	SDL_Rect rect = { x, y };
	textSurface = TTF_RenderUTF8_Blended(font, text, white);
	rect.w = textSurface->w;
	rect.h = textSurface->h;
	SDL_UpperBlitScaled(textSurface, NULL, windowSurface, &rect);
	SDL_FreeSurface(textSurface);
}

inline void Client::writeLine(const char* text, int x, int y, bool large)
{
	TTF_Font* font = nullptr;
	if (large)
		font = uiFontLarge;
	else
		font = uiFont;
	SDL_Rect rect = { x, y };
	textSurface = TTF_RenderUTF8_Blended(font, text, white);
	rect.w = textSurface->w;
	rect.h = textSurface->h;
	SDL_UpperBlitScaled(textSurface, NULL, windowSurface, &rect);
	SDL_FreeSurface(textSurface);
}

inline void Client::writeLine(std::string text, int x, int y, bool large)
{
	TTF_Font* font = nullptr;
	if (large)
		font = uiFontLarge;
	else
		font = uiFont;
	SDL_Rect rect = { x, y };
	textSurface = TTF_RenderUTF8_Blended(font, text.c_str(), white);
	rect.w = textSurface->w;
	rect.h = textSurface->h;
	SDL_UpperBlitScaled(textSurface, NULL, windowSurface, &rect);
	SDL_FreeSurface(textSurface);
}

inline void Client::cleanUp()
{
	SDL_FreeSurface(frameSurface);
}

