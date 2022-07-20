#pragma once
#include"stdafx.h"

constexpr UINT STREAM_MAX_DATA_SIZE = 40960 - 64;

struct CommandPackage
{
	UINT64 timestamp = 0;
	char forward = 0;
	char slipping = 0;
	char heading = 0;
};
struct StreamPackage
{
	UINT64 dataSize = 0;
	BYTE data[STREAM_MAX_DATA_SIZE];
};

UINT64 getCurrentTimestamp();

void streamPackageClear(StreamPackage& package);