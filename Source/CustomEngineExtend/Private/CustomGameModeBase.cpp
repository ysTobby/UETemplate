#include "CustomGameModeBase.h"

ACustomGameModeBase::ACustomGameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ACustomGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	AGameModeBase::InitGame(MapName, Options, ErrorMessage);
}
