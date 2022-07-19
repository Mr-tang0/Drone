#include"DataManager.h"
#include"stdafx.h"

void getCurrentTimestamp(CommandPackage& dataPack)
{
	UINT64 timeStamp;
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	tm temptm =
	{
		sysTime.wSecond,
		sysTime.wMinute,
		sysTime.wHour,
		sysTime.wDay,
		sysTime.wMonth - 1,
		sysTime.wYear - 1900
	};
	timeStamp = mktime(&temptm) * 1000 + sysTime.wMilliseconds;
	dataPack.timestamp = timeStamp;
}

void streamPackageClear(StreamPackage& package)
{
	package.dataSize = 0;
	memset(package.data, 0, STREAM_MAX_DATA_SIZE);
}
