#include "CustomGameInstance.h"
#include "Engine/Engine.h"

#include "Net/ReplayPlaylistTracker.h"
#include "GameMapsSettings.h"
#include "Misc/MessageDialog.h"

UCustomGameInstance::UCustomGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UCustomGameInstance::Init()
{
	UGameInstance::Init();
}

void UCustomGameInstance::StartGameInstance()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(UGameInstance::StartGameInstance);
	UEngine* const Engine = GetEngine();

	// Create default URL.
	// @note: if we change how we determine the valid start up map update LaunchEngineLoop's GetStartupMap()
	FURL DefaultURL;
	DefaultURL.LoadURLConfig(TEXT("DefaultPlayer"), GGameIni);

	// Enter initial world.
	EBrowseReturnVal::Type BrowseRet = EBrowseReturnVal::Failure;
	FString Error;

	const TCHAR* Tmp = FCommandLine::Get();

#if UE_BUILD_SHIPPING && !UE_SERVER && !UE_ALLOW_MAP_OVERRIDE_IN_SHIPPING && !ENABLE_PGO_PROFILE
	// In shipping don't allow a map override unless on server, or running PGO profiling
	Tmp = TEXT("");
#endif // UE_BUILD_SHIPPING && !UE_SERVER && !UE_ALLOW_MAP_OVERRIDE_IN_SHIPPING && !ENABLE_PGO_PROFILE

#if !UE_SERVER
	// Parse replay name if specified on cmdline
	FString ReplayCommand;
	if (FParse::Value(Tmp, TEXT("-REPLAY="), ReplayCommand))
	{
		if (PlayReplay(ReplayCommand))
		{
			return;
		}
	}
	else if (FParse::Value(Tmp, TEXT("-REPLAYPLAYLIST="), ReplayCommand, false))
	{
		FReplayPlaylistParams Params;
		if (ReplayCommand.ParseIntoArray(Params.Playlist, TEXT(",")))
		{
			if (PlayReplayPlaylist(Params))
			{
				return;
			}
		}
	}
#endif // !UE_SERVER

	FString PackageName = "/Game/Maps/Map_Empty?GAME=/Script/CustomEngineExtend.CustomGameModeBaseTest";

	FURL URL(&DefaultURL, *PackageName, TRAVEL_Partial);
	if (URL.Valid)
	{
		BrowseRet = Engine->Browse(*WorldContext, URL, Error);
	}

	// Handle failure.
	if (BrowseRet == EBrowseReturnVal::Failure)
	{
		//UE_LOG(LogLoad, Error, TEXT("%s"), *FString::Printf(TEXT("Failed to enter %s: %s. Please check the log for errors."), *DefaultMap, *Error));
		const FText Message = FText::Format(NSLOCTEXT("Engine", "DefaultMapNotFound", "The Init map could not be found. Exiting."));
		FMessageDialog::Open(EAppMsgType::Ok, Message);
		FPlatformMisc::RequestExit(false);
		return;
	}

	BroadcastOnStart();
}
