#include "CustomGameModeBaseTest.h"

ACustomGameModeBaseTest::ACustomGameModeBaseTest(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ACustomGameModeBaseTest::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	AGameModeBase::InitGame(MapName, Options, ErrorMessage);
}
