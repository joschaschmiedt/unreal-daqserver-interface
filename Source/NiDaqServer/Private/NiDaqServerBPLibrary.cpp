// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "NiDaqServerBPLibrary.h"
#include "NiDaqServer.h"
#include "DaqServerInterface.h"
#include "Windows/MinWindows.h"


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
	//ensureMsgf(DaqServerInterface::StopDaqserverProcess() == 0,
		//TEXT("Stop of NidaqServer.exe failed"));

}

void UNiDaqServerBPLibrary::SendEventmarker(int code)
{
	UE_LOG(LogTemp, Display, TEXT("Send eventmarker %d"), code);
	ensureAlwaysMsgf(DaqServerInterface::SendEventmarker(unsigned short(code)) == 0,
		TEXT("Could not send eventmarker"));
}
