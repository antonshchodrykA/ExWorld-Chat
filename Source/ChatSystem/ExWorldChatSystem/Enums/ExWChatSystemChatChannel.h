#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(Blueprintable, BlueprintType)
enum class EExWChatSystemChatChannel : uint8
{
	MacroZone =     0 UMETA(DisplayName = "MacroZone"),
	Closer =   1 UMETA(DisplayName = "Closer"),
	Race =   2 UMETA(DisplayName = "Race"),
	Party =   3 UMETA(DisplayName = "Party"),
	Guild =   4 UMETA(DisplayName = "Guild"),
	WhisperToPlayerName =   5 UMETA(DisplayName = "WhisperToPlayerName"),
	ReplyWhisperToPlayerName =   6 UMETA(DisplayName = "ReplyWhisperToPlayerName"),
	Global = 7 UMETA(DisplayName = "Global"),
	None = 8 UMETA(DisplayName = "None"),
};
