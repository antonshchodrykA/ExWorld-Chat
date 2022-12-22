#include "ChatSystemCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "ChatSystem/ExWorldChatSystem/Interfaces/ExWChatSystemMessageReceiverInterface.h"
#include "ChatSystem/ExWorldChatSystem/Utils/ExWChatSystemUtils.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"


#pragma region Setup

AChatSystemCharacter::AChatSystemCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AChatSystemCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AChatSystemCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AChatSystemCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AChatSystemCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AChatSystemCharacter::LookUpAtRate);
}

void AChatSystemCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AChatSystemCharacter, PlayerName);
	DOREPLIFETIME(AChatSystemCharacter, bEnableHeadBubbleChat);
}

#

#pragma endregion


#pragma region Input

void AChatSystemCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AChatSystemCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AChatSystemCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AChatSystemCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

#pragma endregion


#pragma region ChatSystem

#pragma region UtilsFunctions

void AChatSystemCharacter::ServerSetPlayerName_Implementation(const FString& Value)
{
	PlayerName = Value;
}

void AChatSystemCharacter::SetRandomMacroZone()
{
	TArray<FString> TestList;
	TestList.Add("Zone 01");
	TestList.Add("Zone 02");
	TestList.Add("Zone 03");
	TestList.Add("Zone 04");
	TestList.Add("Zone 05");

	const int32 RandomIndex = FMath::RandRange(0, TestList.Num() - 1);

	ServerSetRandomMacroZone(TestList[RandomIndex]);
}

void AChatSystemCharacter::ServerSetRandomMacroZone_Implementation(const FString& Value)
{
	NetMulticastSetRandomMacroZone(Value);
}

void AChatSystemCharacter::NetMulticastSetRandomMacroZone_Implementation(const FString& Value)
{
	MacroZone = Value;

	if (IsLocallyControlled())
		OnTestCharacterValuesUpdated();
}

void AChatSystemCharacter::SetRandomRace()
{
	TArray<FString> TestList;
	TestList.Add("Race 01");
	TestList.Add("Race 02");
	TestList.Add("Race 03");
	TestList.Add("Race 04");
	TestList.Add("Race 05");

	const int32 RandomIndex = FMath::RandRange(0, TestList.Num() - 1);

	ServerSetRandomRace(TestList[RandomIndex]);
}

void AChatSystemCharacter::ServerSetRandomRace_Implementation(const FString& Value)
{
	NetMulticastSetRandomRace(Value);
}

void AChatSystemCharacter::NetMulticastSetRandomRace_Implementation(const FString& Value)
{
	Race = Value;

	if (IsLocallyControlled())
		OnTestCharacterValuesUpdated();
}


void AChatSystemCharacter::SetRandomParty()
{
	TArray<FString> TestList;
	TestList.Add("Party 01");
	TestList.Add("Party 02");
	TestList.Add("Party 03");
	TestList.Add("Party 04");
	TestList.Add("Party 05");

	const int32 RandomIndex = FMath::RandRange(0, TestList.Num() - 1);

	ServerSetRandomParty(TestList[RandomIndex]);
}

void AChatSystemCharacter::ServerSetRandomParty_Implementation(const FString& Value)
{
	NetMulticastSetRandomParty(Value);
}

void AChatSystemCharacter::NetMulticastSetRandomParty_Implementation(const FString& Value)
{
	Party = Value;

	if (IsLocallyControlled())
		OnTestCharacterValuesUpdated();
}


void AChatSystemCharacter::SetRandomGuild()
{
	TArray<FString> TestList;
	TestList.Add("Guild 01");
	TestList.Add("Guild 02");
	TestList.Add("Guild 03");
	TestList.Add("Guild 04");
	TestList.Add("Guild 05");

	const int32 RandomIndex = FMath::RandRange(0, TestList.Num() - 1);

	ServerSetRandomGuild(TestList[RandomIndex]);
}

void AChatSystemCharacter::ServerSetRandomGuild_Implementation(const FString& Value)
{
	NetMulticastSetRandomGuild(Value);
}

void AChatSystemCharacter::NetMulticastSetRandomGuild_Implementation(const FString& Value)
{
	Guild = Value;

	if (IsLocallyControlled())
		OnTestCharacterValuesUpdated();
}

void AChatSystemCharacter::OnTestCharacterValuesUpdated_Implementation()
{
}
#pragma endregion


void AChatSystemCharacter::ClientSendMessage_Implementation(AChatSystemCharacter* Sender, const FString& Command,
                                                            const FString& Message, const FString& TargetPlayerName)
{
	ServerSendMessage(Sender, Command, Message, TargetPlayerName);
}

void AChatSystemCharacter::ServerSendMessage_Implementation(AChatSystemCharacter* Sender, const FString& Command,
                                                            const FString& Message, const FString& TargetPlayerName)
{
	NetMulticastSendMessage(Sender, Command, Message, TargetPlayerName);
}

void AChatSystemCharacter::NetMulticastSendMessage_Implementation(AChatSystemCharacter* Sender, const FString& Command, const FString& Message,const FString& TargetPlayerName)
{
	if (!UKismetSystemLibrary::IsDedicatedServer(this))
		OnNewMessageReceived(Sender, Command, Message, TargetPlayerName);
}

void AChatSystemCharacter::OnNewMessageReceived_Implementation(AChatSystemCharacter* Sender, const FString& Command,
                                                               const FString& Message, const FString& TargetPlayerName)
{
	const AChatSystemCharacter* LocalPlayerCharacter = Cast<AChatSystemCharacter>(
		UGameplayStatics::GetPlayerPawn(this, 0));

	if (LocalPlayerCharacter)
	{
		const EExWChatSystemChatChannel MessageChannel = UExWChatSystemUtils::GetChatChannelByCommand(Command);

		if (Sender != LocalPlayerCharacter)
		{
			switch (MessageChannel)
			{
			case EExWChatSystemChatChannel::MacroZone:

				if (LocalPlayerCharacter->MacroZone != Sender->MacroZone)
					return;

				break;

			case EExWChatSystemChatChannel::Closer:
				{
					const FVector LocalPlayerLocation = LocalPlayerCharacter->GetActorLocation();
					const FVector SenderPlayerLocation = Sender->GetActorLocation();

					if (UKismetMathLibrary::Vector_Distance(LocalPlayerLocation, SenderPlayerLocation) > 500)
						return;
				}

				break;
			case EExWChatSystemChatChannel::Race:

				if (LocalPlayerCharacter->Race != Sender->Race)
					return;

				break;
			case EExWChatSystemChatChannel::Party:

				if (LocalPlayerCharacter->Party != Sender->Party)
					return;

				break;
			case EExWChatSystemChatChannel::Guild:

				if (LocalPlayerCharacter->Guild != Sender->Guild)
					return;

				break;
			case EExWChatSystemChatChannel::WhisperToPlayerName:

				if (TargetPlayerName != LocalPlayerCharacter->PlayerName)
					return;
				
				break;
			case EExWChatSystemChatChannel::ReplyWhisperToPlayerName:
				
				if (TargetPlayerName != LocalPlayerCharacter->PlayerName)
					return;

				break;
			case EExWChatSystemChatChannel::Global:

				break;
			case EExWChatSystemChatChannel::None:

				break;
			default:
				break;
			}
		}


		if (LocalPlayerCharacter->ChatUserWidget != nullptr && LocalPlayerCharacter->ChatUserWidget->GetClass()->ImplementsInterface(UExWChatSystemMessageReceiverInterface::StaticClass()))
			IExWChatSystemMessageReceiverInterface::Execute_OnNewMessage(LocalPlayerCharacter->ChatUserWidget, Command,Sender->PlayerName, Message, MessageChannel,TargetPlayerName);

		if(bEnableHeadBubbleChat && (MessageChannel==EExWChatSystemChatChannel::Party||MessageChannel==EExWChatSystemChatChannel::Guild))
			OnShowMessageInBubble(Message);
			
	}
}

void AChatSystemCharacter::OnShowMessageInBubble_Implementation(const FString& Message)
{
	
}

void AChatSystemCharacter::SetHeadBubbleChatEnabled_Implementation(bool Value)
{
	bEnableHeadBubbleChat = Value;
}
#pragma endregion
