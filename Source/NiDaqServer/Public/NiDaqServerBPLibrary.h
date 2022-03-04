// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "NiDaqServerBPLibrary.generated.h"


/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
UCLASS()
class UNiDaqServerBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "NiDaqServer")
		static void GiveReward();

	UFUNCTION(BlueprintCallable, Category = "NiDaqServer")
		static void GiveRewardOfLength(int duration);

	UFUNCTION(BlueprintCallable, Category = "NiDaqServer")
		static void Connect();

	UFUNCTION(BlueprintCallable, Category = "NiDaqServer")
		static void Disconnect();

	UFUNCTION(BlueprintCallable, Category = "NiDaqServer")
		static void StartNidaqServerProcess();

	UFUNCTION(BlueprintCallable, Category = "NiDaqServer")
		static void StopNidaqServerProcess();

	UFUNCTION(BlueprintCallable, Category = "NiDaqServer")
		static bool isNidaqServerProcessRunning();

	UFUNCTION(BlueprintCallable, Category = "NiDaqServer")
		static int64 SendEventmarker(int code);

	UFUNCTION(BlueprintCallable, Category = "NiDaqServer")
		static void AddPulseEvent(int linenumber, FString pulseEventName);
	
	UFUNCTION(BlueprintCallable, Category = "NiDaqServer")
		static void AddOnOffEvents(int linenumber, FString onEventName, FString offEventName);

	UFUNCTION(BlueprintCallable, Category = "NiDaqServer")
		static bool AddOutputLinePulse(int linenumber, FString pulseOutName);

	UFUNCTION(BlueprintCallable, Category = "NiDaqServer")
		static bool SetOutputPulseDuration(int linenumber, int timems);

	UFUNCTION(BlueprintCallable, Category = "NiDaqServer")
		static bool OutputPulse(FString pulseOutName);

	UFUNCTION(BlueprintCallable, Category = "NiDaqServer")
		static void StartTracking();

	UFUNCTION(BlueprintCallable, Category = "NiDaqServer")
		static bool IsEventSignaled(FString name);

	UFUNCTION(BlueprintCallable, Category = "NiDaqServer")
		static bool WaitForDigitalEvent(int duration, FString name);

	static FProcHandle hProcess;
};
