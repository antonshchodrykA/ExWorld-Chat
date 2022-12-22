// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChatSystem/ExWorldChatSystem/Enums/ExWChatSystemChatChannel.h"
#include "ChatSystem/ExWorldChatSystem/Structs/ExWChatSystemMessageDataContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ExWChatSystemUtils.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class CHATSYSTEM_API UExWChatSystemUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static TMap<FString, EExWChatSystemChatChannel> InitChatCommands();

public:
	static TMap<FString, EExWChatSystemChatChannel> ChatCommands;

public:
	UFUNCTION(BlueprintCallable)
	static EExWChatSystemChatChannel GetChatChannelByCommand(const FString Command);

	UFUNCTION(BlueprintCallable)
	static TArray<FExWChatSystemMessageData>  AddMessageDataToMessagesContainer(TArray<FExWChatSystemMessageData>  InData, FExWChatSystemMessageData DataToAdd, int32 MaxMessageDataPerChannel = 10);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static void TryGetCommandOnMessage(FString Message, FString& Command,FString& ForPlayerName, bool& Success);
};
