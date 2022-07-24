#pragma once
#include "stdafx.h"
#include "Net.h"
#include "DataManager.h"

constexpr auto PINGSIZE = 64;
constexpr auto MESSAGESIZE = 4;
constexpr auto FRAMESIZE = 64;

class Client
{
public:
	Client(char* serverIP);
private:
	void loop();
	CommandPackage* commandPack = new CommandPackage;
	uint streamPackSize = 0;
	NetClient netClient;
	

	bool go = true;
	
	std::list<UINT64> pingList;
	std::list<UINT64>::iterator uint64Iter;

	std::list<std::string> messageList;
	std::list<std::string>::iterator stringIter;

	std::list<float> frameList;
	std::list<float>::iterator floatIter;

	std::list<float> packSizeList;

	UINT64 pingSolve();
	UINT64 recvTime;
	UINT64 loopTimeA = 0;
	UINT64 loopTimeB = 0;
	float frameRate;
	float frameSolve();
	float speedSolve();

	
	// SDL Attributes
	void eventSolve();
	inline void writeLine(char* text, int x, int y, bool large=false);
	inline void writeLine(const char* text, int x, int y, bool large = false);
	inline void writeLine(std::string text, int x, int y, bool large = false);
	inline void cleanUp();



	SDL_Window* windowMain = nullptr;
	SDL_Surface* imgSurface = nullptr;
	SDL_Surface* windowSurface = nullptr;
	SDL_Surface* optimizedSurface = nullptr;
	TTF_Font* uiFontLarge = nullptr;
	TTF_Font* uiFont = nullptr;
	SDL_Color white = { 255, 255, 255 };
	SDL_Color black = { 0, 0, 0 };
	SDL_Surface* textSurface = nullptr;
	SDL_Surface* frameSurface = nullptr;
	SDL_Rect frameRect = { 0, 0, 640, 480 };
	SDL_Event event_;

	// OpenCV Attributes
	cv::Mat frame, fliped, buffer;
};

