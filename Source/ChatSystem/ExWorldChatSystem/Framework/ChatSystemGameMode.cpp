#include "ChatSystemGameMode.h"
#include "../Character/ChatSystemCharacter.h"
#include "UObject/ConstructorHelpers.h"

AChatSystemGameMode::AChatSystemGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ExWChatSystem/Blueprints/ExWChatSystemCharacterBase"));

	if (PlayerPawnBPClass.Class != NULL)
		DefaultPawnClass = PlayerPawnBPClass.Class;
}
