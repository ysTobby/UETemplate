#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CustomGameModeBaseTest.generated.h"

/**
 * 
 */
UCLASS()
class ACustomGameModeBaseTest : public AGameModeBase
{
	GENERATED_UCLASS_BODY()
public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
};
