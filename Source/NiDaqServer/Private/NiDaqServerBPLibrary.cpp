// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "NiDaqServerBPLibrary.h"
#include "NiDaqServer.h"
#include "DaqServerInterface.h"
#include "Windows/MinWindows.h"
#include <string>


FProcHandle UNiDaqServerBPLibrary::hProcess = FProcHandle(nullptr);

UNiDaqServerBPLibrary::UNiDaqServerBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}


void UNiDaqServerBPLibrary::GiveReward()
{
	UE_LOG(LogTemp, Display, TEXT("GiveReward by event"));
	ensureAlwaysMsgf(DaqServerInterface::GiveReward() == 0,
		TEXT("Could not issue reward by event"));
}

void UNiDaqServerBPLibrary::GiveRewardOfLength(int duration)
{
	UE_LOG(LogTemp, Display, TEXT("GiveReward of duration %d"), duration);

	unsigned short sequence[3] = { 0 };
	sequence[0] = duration;

	ensureAlwaysMsgf(DaqServerInterface::GiveReward(&sequence[0], (unsigned short)1) == 0,
		TEXT("Could not issue reward of length %d"), duration);
}

void UNiDaqServerBPLibrary::Connect()
{
	UE_LOG(LogTemp, Display, TEXT("Connect to DaqServer"));
	ensureMsgf(DaqServerInterface::Connect() == 0,
		TEXT("Could not connect to DaqServer"));
}

void UNiDaqServerBPLibrary::Disconnect()
{
	UE_LOG(LogTemp, Display, TEXT("Disconnect from DaqServer"));	
	if (DaqServerInterface::Disconnect() != S_OK) {
		UE_LOG(LogTemp, Warning, TEXT("Disconnect from DaqServer may have failed"));
	}	
}

void UNiDaqServerBPLibrary::StartNidaqServerProcess()
{	
	if (FPlatformProcess::IsApplicationRunning(TEXT("NidaqServer.exe"))) {
		UE_LOG(LogTemp, Warning, TEXT("NidaqServer.exe is already running. Stopping the process from Unreal may not work."));
		return;
	}
	
	UE_LOG(LogTemp, Display, TEXT("Starting NiDaqServer"));
	UNiDaqServerBPLibrary::hProcess = FPlatformProcess::CreateProc(TEXT("NidaqServer.exe"),
		TEXT(""), false, false, false, nullptr, 0, nullptr, nullptr);

	HANDLE hEvent = CreateEventA(nullptr, false, false, "DaqServerDone");
	DWORD result = WaitForSingleObject(hEvent, DWORD(10000));

}

void UNiDaqServerBPLibrary::StopNidaqServerProcess()
{
	UE_LOG(LogTemp, Display, TEXT("Stopping NiDaqServer"));
	FPlatformProcess::TerminateProc(UNiDaqServerBPLibrary::hProcess);
	FPlatformProcess::WaitForProc(UNiDaqServerBPLibrary::hProcess);
	FPlatformProcess::CloseProc(UNiDaqServerBPLibrary::hProcess);
	//ensureMsgf(DaqServerInterface::StopDaqserverProcess() == 0,//TEXT("Stop of NidaqServer.exe failed"));
}

bool UNiDaqServerBPLibrary::isNidaqServerProcessRunning()
{
	
	return FPlatformProcess::IsProcRunning(UNiDaqServerBPLibrary::hProcess);

}

int64 UNiDaqServerBPLibrary::SendEventmarker(int code)
{
	UE_LOG(LogTemp, Display, TEXT("Send eventmarker %d"), code);
	ensureAlwaysMsgf(DaqServerInterface::SendEventmarker(unsigned short(code)) == 0,
		TEXT("Could not send eventmarker"));

	LARGE_INTEGER ticks;
	QueryPerformanceCounter(&ticks);
	return ticks.QuadPart;
}

void UNiDaqServerBPLibrary::AddPulseEvent(int linenumber, FString pulseEventName)
{
	UE_LOG(LogTemp, Display, TEXT("Add line %d to monitor digital pulse events %s"), linenumber, *pulseEventName);
	ensureAlwaysMsgf(
		DaqServerInterface::AddLinePulse(
			BYTE(linenumber),
			std::string(TCHAR_TO_UTF8(*pulseEventName))) == 0,
		TEXT("Daqserver: could not add pulse event"));
}

void UNiDaqServerBPLibrary::AddOnOffEvents(int linenumber, FString onEventName, FString offEventName)
{
	UE_LOG(LogTemp, Display, TEXT("Add line %d to monitor for digital reset events %s and %s"), linenumber, *onEventName, *offEventName);
	ensureAlwaysMsgf(
		DaqServerInterface::AddLineOnOff(
			BYTE(linenumber),
			std::string(TCHAR_TO_UTF8(*onEventName)),
			std::string(TCHAR_TO_UTF8(*offEventName))) == 0,
		TEXT("Daqserver: could not add onoff event"));
}

bool UNiDaqServerBPLibrary::AddOutputLinePulse(int linenumber, FString pulseOutName)
{
	if (linenumber == 3) {
		ensureAlwaysMsgf(false, TEXT("Daqserver: digital line %d is reserved for reward"), linenumber);
		return false;
	}
	else if (linenumber == 7) {
		ensureAlwaysMsgf(false, TEXT("Daqserver: digital line %d is reserved for eventmarkers"), linenumber);
		return false;
	}
	
	UE_LOG(LogTemp, Display, TEXT("Add line %d to generate pulse output %s"), linenumber, *pulseOutName);
	if (ensureAlwaysMsgf(
		DaqServerInterface::AddOutputLinePulse(
			BYTE(linenumber),
			std::string(TCHAR_TO_UTF8(*pulseOutName))) == 0,
		TEXT("Daqserver: could not add pulse event"))) {
		return true;
	}
	else { return false; }
}

bool UNiDaqServerBPLibrary::SetOutputPulseDuration(int linenumber, int timems)
{
	if (ensureAlwaysMsgf(
		DaqServerInterface::SetPulseDuration(BYTE(linenumber), unsigned short(timems)) == 0,
		TEXT("Daqserver: set pulse duration of line %d"), linenumber)) {
		return true;
	}
	else { return false; }
}

bool UNiDaqServerBPLibrary::OutputPulse(FString pulseOutName)
{	
	HANDLE hEvent = CreateEventA(NULL, FALSE, FALSE, (std::string(TCHAR_TO_UTF8(*pulseOutName)) + "\0").c_str() );
	bool success = SetEvent(hEvent);	
	if (success) { 
		UE_LOG(LogTemp, Display, TEXT("%s sent"), *pulseOutName);
		return true;
	}
	else { 
		ensureAlwaysMsgf(false, TEXT("Daqserver: %s was not sent, error: %d"), *pulseOutName, GetLastError());
		return false;
	}
}


void UNiDaqServerBPLibrary::StartTracking()
{
	// Once this is started you cannot add any more lines
	UE_LOG(LogTemp, Display, TEXT("Starting digital input monitoring"));
	ensureAlwaysMsgf(DaqServerInterface::StartTrackingLines() == 0, 
		TEXT("Daqserver: could not start digital input tracking"));
}

bool UNiDaqServerBPLibrary::IsEventSignaled(FString name)
{	
	HANDLE hEvent = CreateEventA(NULL, FALSE, FALSE, (std::string(TCHAR_TO_UTF8(*name)) + "\0").c_str() );
	DWORD result = WaitForSingleObject(hEvent, 0);
	if (result == WAIT_OBJECT_0) {
		UE_LOG(LogTemp, Display, TEXT("Daqserver: digital input %s is signaled"), *name);
		return true;
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("Daqserver: digital input %s is unsignaled"), *name);
		return false;
	}	
}

bool UNiDaqServerBPLibrary::WaitForDigitalEvent(int duration, FString name)
{
	HANDLE hEvent = CreateEventA(NULL, FALSE, FALSE, (std::string(TCHAR_TO_UTF8(*name)) + "\0").c_str() );
	DWORD result = WaitForSingleObject(hEvent, DWORD(duration));
	switch (result) 
    { 
        case WAIT_OBJECT_0: 
            UE_LOG(LogTemp, Display, TEXT("Daqserver: digital event %s triggered"), *name);
			return true;
        case WAIT_TIMEOUT:
			UE_LOG(LogTemp, Display, TEXT("Daqserver: Wait timeout without %s event"), *name);
			return false;
        default: 
            ensureAlwaysMsgf(false, TEXT("Daqserver: Wait error: %d"), GetLastError());
			return false;
    }

}
