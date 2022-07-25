#include"DataManager.h"
#include"stdafx.h"

UINT64 getCurrentTimestamp()
{
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
	return mktime(&temptm) * 1000 + sysTime.wMilliseconds;;
}
