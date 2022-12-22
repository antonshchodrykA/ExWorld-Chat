#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "../Enums/ExWChatSystemChatChannel.h"
#include "ExWChatSystemMessageData.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable, BlueprintType)
struct FExWChatSystemMessageData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString From;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Message;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EExWChatSystemChatChannel Channel;
};
