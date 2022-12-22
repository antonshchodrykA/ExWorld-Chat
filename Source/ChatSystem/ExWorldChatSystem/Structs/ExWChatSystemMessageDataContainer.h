#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "ExWChatSystemMessageData.h"
#include "ExWChatSystemMessageDataContainer.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable, BlueprintType)
struct FExWChatSystemMessageDataContainer
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FExWChatSystemMessageData> Data;

	
};
