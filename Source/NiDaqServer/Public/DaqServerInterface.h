#pragma once

#include "Windows/MinWindows.h"
#define BUFFERSIZE 32

union COMMANDBUFFER {
	unsigned char command[BUFFERSIZE];
	struct {
		BYTE type;
		unsigned char body[BUFFERSIZE - 1];
	};
};
class DaqServerInterface
{
public:
	static DWORD Connect();
	static DWORD Disconnect();
	static DWORD SendEventmarker(unsigned short markercode);	
	static DWORD GiveReward();
	static DWORD GiveReward(unsigned short * pTimes, unsigned short nTimes);
    static DWORD SetRewardTime(unsigned short int timems);
	static DWORD GetTotalRewardTime(unsigned long int * totalTime);
	static DWORD AddLinePulse(unsigned short linenumber);
	static DWORD AddLineOnOff(unsigned short linenumber);
	static DWORD StartTrackingLine();
	static DWORD StartDaqserverProcess();
	static DWORD StopDaqserverProcess();

private:
	static HANDLE hPipe;
	static HANDLE hRewardEvent;
	static HANDLE hRewardDoneEvent;
	static HANDLE hDaqServerDoneEvent;
	static PROCESS_INFORMATION processInformation;
	static STARTUPINFOA startupInfo;	
};

