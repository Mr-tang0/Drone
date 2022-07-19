#pragma once
#include "stdafx.h"
#include "NetworkClient.h"
class Client
{
public:
	Client();
private:
	void loop();
	CommandPackage* commandPack = new CommandPackage;
	StreamPackage* streamPack = new StreamPackage;
	NetworkClient client;
	ULONG64 tick = 0;

	
	// SDL Attributes
	void eventSolve();
	void writeLine(char* text, int x, int y);
	void writeLine(std::string text, int x, int y);
	void cleanUp();

	SDL_Window* windowMain = nullptr;
	SDL_Surface* imgSurface = nullptr;
	SDL_Surface* windowSurface = nullptr;
	SDL_Surface* optimizedSurface = nullptr;
	TTF_Font* uiFont = nullptr;
	SDL_Color white = { 255, 255, 255 };
	SDL_Color black = { 0, 0, 0 };
	SDL_Surface* textSurface = nullptr;
	SDL_Surface* frameSurface = nullptr;
	SDL_Rect frameRect = { 0, 0, 640, 480 };

	cv::Mat frame, fliped;
};

