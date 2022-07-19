#pragma once
#include"stdafx.h"

constexpr UINT STREAM_MAX_DATA_SIZE = 40960 - 64;

struct CommandPackage
{
	UINT64 timestamp;
	char forward;
	char slipping;
	char heading;
};
struct StreamPackage
{
	UINT64 dataSize;
	BYTE data[STREAM_MAX_DATA_SIZE];
};

void getCurrentTimestamp(CommandPackage& dataPack);

void streamPackageClear(StreamPackage& package);