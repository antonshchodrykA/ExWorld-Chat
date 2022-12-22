// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ExWChatSystemMessageReceiverInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UExWChatSystemMessageReceiverInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CHATSYSTEM_API IExWChatSystemMessageReceiverInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		 void OnNewMessage(const FString& Command, const FString& PlayerName, const FString& Message, EExWChatSystemChatChannel MessageChannel,const FString& TargetPlayerName);
};
