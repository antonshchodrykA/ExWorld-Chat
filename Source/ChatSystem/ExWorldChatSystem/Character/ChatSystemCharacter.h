// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/UserWidget.h"
#include "ChatSystemCharacter.generated.h"

UCLASS(config=Game)
class AChatSystemCharacter : public ACharacter
{
	GENERATED_BODY()

#pragma region Setup

public:
	AChatSystemCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

#pragma endregion

#pragma region Components

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

#pragma endregion

#pragma region BasicProperties

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

#pragma endregion

#pragma region Input

protected:
	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

#pragma endregion


#pragma region ChatSystem

public: //Default character properties

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MacroZone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Race;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Party;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Guild;

public: //Utils functions

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void ServerSetPlayerName(const FString& Value);

	UFUNCTION(BlueprintCallable)
	void SetRandomMacroZone();

	UFUNCTION(Server, Reliable)
	void ServerSetRandomMacroZone(const FString& Value);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastSetRandomMacroZone(const FString& Value);


	UFUNCTION(BlueprintCallable)
	void SetRandomRace();

	UFUNCTION(Server, Reliable)
	void ServerSetRandomRace(const FString& Value);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastSetRandomRace(const FString& Value);


	UFUNCTION(BlueprintCallable)
	void SetRandomParty();

	UFUNCTION(Server, Reliable)
	void ServerSetRandomParty(const FString& Value);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastSetRandomParty(const FString& Value);


	UFUNCTION(BlueprintCallable)
	void SetRandomGuild();

	UFUNCTION(Server, Reliable)
	void ServerSetRandomGuild(const FString& Value);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastSetRandomGuild(const FString& Value);

	UFUNCTION(BlueprintNativeEvent)
	void OnTestCharacterValuesUpdated();

public: //Chat system Properties


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UUserWidget* ChatUserWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Replicated)
	bool bEnableHeadBubbleChat = true;

public: //Functions

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void ClientSendMessage(AChatSystemCharacter* Sender, const FString& Command, const FString& Message,
	                       const FString& TargetPlayerName);

	UFUNCTION(Server, Reliable)
	void ServerSendMessage(AChatSystemCharacter* Sender, const FString& Command, const FString& Message,
	                       const FString& TargetPlayerName);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastSendMessage(AChatSystemCharacter* Sender, const FString& Command, const FString& Message,
	                             const FString& TargetPlayerName);

	UFUNCTION(BlueprintNativeEvent)
	void OnNewMessageReceived(AChatSystemCharacter* Sender, const FString& Command, const FString& Message,
	                          const FString& TargetPlayerName);

	UFUNCTION(BlueprintNativeEvent)
	void OnShowMessageInBubble(const FString& Message);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void SetHeadBubbleChatEnabled(bool Value);

#pragma endregion
};
