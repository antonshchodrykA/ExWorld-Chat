#include "ExWChatSystemUtils.h"

TMap<FString, EExWChatSystemChatChannel> UExWChatSystemUtils::ChatCommands = UExWChatSystemUtils::InitChatCommands();

TMap<FString, EExWChatSystemChatChannel> UExWChatSystemUtils::InitChatCommands()
{
	TMap<FString, EExWChatSystemChatChannel> RetVal;

	RetVal.Add("/1", EExWChatSystemChatChannel::MacroZone);

	RetVal.Add("/say", EExWChatSystemChatChannel::Closer);
	RetVal.Add("/y", EExWChatSystemChatChannel::Closer);

	RetVal.Add("/r", EExWChatSystemChatChannel::Race);

	RetVal.Add("/p", EExWChatSystemChatChannel::Party);
	RetVal.Add("/party", EExWChatSystemChatChannel::Party);

	RetVal.Add("/g", EExWChatSystemChatChannel::Guild);
	RetVal.Add("/guild", EExWChatSystemChatChannel::Guild);

	RetVal.Add("/w", EExWChatSystemChatChannel::WhisperToPlayerName);
	RetVal.Add("/whisper", EExWChatSystemChatChannel::WhisperToPlayerName);

	RetVal.Add("/rr", EExWChatSystemChatChannel::ReplyWhisperToPlayerName);

	RetVal.Add("/global", EExWChatSystemChatChannel::Global);

	return RetVal;
}

EExWChatSystemChatChannel UExWChatSystemUtils::GetChatChannelByCommand(const FString Command)
{
	if (ChatCommands.Contains(Command))
		return *ChatCommands.Find(Command);

	return EExWChatSystemChatChannel::None;
}

TArray<FExWChatSystemMessageData> UExWChatSystemUtils::AddMessageDataToMessagesContainer(
	TArray<FExWChatSystemMessageData> InData, FExWChatSystemMessageData DataToAdd, int32 MaxMessageDataPerChannel)
{
	InData.Add(DataToAdd);


	TArray<FExWChatSystemMessageData> NewDataList;

	int32 MacroZoneDataCount = 0;
	int32 CloserDataCount = 0;
	int32 RaceDataCount = 0;
	int32 PartyDataCount = 0;
	int32 GuildDataCount = 0;
	int32 WhisperToPlayerNameDataCount = 0;
	int32 ReplyWhisperToPlayerNameDataCount = 0;
	int32 GlobalDataCount = 0;

	Algo::Reverse(InData);

	for (int32 i = 0; i < InData.Num(); i++)
	{
		switch (InData[i].Channel)
		{
		case EExWChatSystemChatChannel::MacroZone:
			MacroZoneDataCount++;

			if (MacroZoneDataCount < MaxMessageDataPerChannel)
				NewDataList.Add(InData[i]);

			break;
		case EExWChatSystemChatChannel::Closer:
			CloserDataCount++;

			if (CloserDataCount < MaxMessageDataPerChannel)
				NewDataList.Add(InData[i]);

			break;
		case EExWChatSystemChatChannel::Race:
			RaceDataCount++;

			if (RaceDataCount < MaxMessageDataPerChannel)
				NewDataList.Add(InData[i]);

			break;
		case EExWChatSystemChatChannel::Party:
			PartyDataCount++;

			if (PartyDataCount < MaxMessageDataPerChannel)
				NewDataList.Add(InData[i]);

			break;
		case EExWChatSystemChatChannel::Guild:
			GuildDataCount++;

			if (GuildDataCount < MaxMessageDataPerChannel)
				NewDataList.Add(InData[i]);

			break;
		case EExWChatSystemChatChannel::WhisperToPlayerName:
			WhisperToPlayerNameDataCount++;

			if (WhisperToPlayerNameDataCount < MaxMessageDataPerChannel)
				NewDataList.Add(InData[i]);

			break;
		case EExWChatSystemChatChannel::ReplyWhisperToPlayerName:
			ReplyWhisperToPlayerNameDataCount++;

			if (ReplyWhisperToPlayerNameDataCount < MaxMessageDataPerChannel)
				NewDataList.Add(InData[i]);

			break;
		case EExWChatSystemChatChannel::Global:
			GlobalDataCount++;

			if (GlobalDataCount < MaxMessageDataPerChannel)
				NewDataList.Add(InData[i]);

			break;
		case EExWChatSystemChatChannel::None:
			break;
		default: ;
		}
	}

	Algo::Reverse(NewDataList);

	InData = NewDataList;


	return InData;
}

void UExWChatSystemUtils::TryGetCommandOnMessage(FString Message, FString& Command, FString& ForPlayerName,
                                                 bool& Success)
{
	TArray<FString> SeparatedBySpaceStringArray;

	Message.ParseIntoArray(SeparatedBySpaceStringArray,TEXT(" "));

	if (SeparatedBySpaceStringArray.Num() > 0 && SeparatedBySpaceStringArray[0] == "/1")
	{
		Command = "/1";
		Success = true;
		return;
	}

	if (SeparatedBySpaceStringArray.Num() > 0 && SeparatedBySpaceStringArray[0] == "/say")
	{
		Command = "/say";
		Success = true;
		return;
	}

	if (SeparatedBySpaceStringArray.Num() > 0 && SeparatedBySpaceStringArray[0] == "/y")
	{
		Command = "/y";
		Success = true;
		return;
	}

	if (SeparatedBySpaceStringArray.Num() > 0 && SeparatedBySpaceStringArray[0] == "/r")
	{
		Command = "/r";
		Success = true;
		return;
	}

	if (SeparatedBySpaceStringArray.Num() > 0 && SeparatedBySpaceStringArray[0] == "/p")
	{
		Command = "/p";
		Success = true;
		return;
	}

	if (SeparatedBySpaceStringArray.Num() > 0 && SeparatedBySpaceStringArray[0] == "/party")
	{
		Command = "/party";
		Success = true;
		return;
	}

	if (SeparatedBySpaceStringArray.Num() > 0 && SeparatedBySpaceStringArray[0] == "/g")
	{
		Command = "/g";
		Success = true;
		return;
	}

	if (SeparatedBySpaceStringArray.Num() > 0 && SeparatedBySpaceStringArray[0] == "/guild")
	{
		Command = "/guild";
		Success = true;
		return;
	}

	if (SeparatedBySpaceStringArray.Num() > 2 && SeparatedBySpaceStringArray[0] == "/w")
	{
		Command = "/w";
		ForPlayerName = SeparatedBySpaceStringArray[1];
		Success = true;
		return;
	}

	if (SeparatedBySpaceStringArray.Num() > 2 && SeparatedBySpaceStringArray[0] == "/whisper")
	{
		Command = "/whisper";
		ForPlayerName = SeparatedBySpaceStringArray[1];
		Success = true;
		return;
	}

	if (SeparatedBySpaceStringArray.Num() > 0 && SeparatedBySpaceStringArray[0] == "/r")
	{
		Command = "/r";
		Success = true;
		return;
	}

	if (SeparatedBySpaceStringArray.Num() > 0 && SeparatedBySpaceStringArray[0] == "/global")
	{
		Command = "/global";
		Success = true;
		return;
	}
	
	Success = false;
	return;
}
