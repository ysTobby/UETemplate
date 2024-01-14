#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CustomGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ACustomGameModeBase : public AGameModeBase
{
	GENERATED_UCLASS_BODY()
public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
};
