#pragma once
#include "Engine/GameInstance.h"
#include "CustomGameInstance.generated.h"

class UPackage;

UCLASS()
class UCustomGameInstance : public UGameInstance
{
	GENERATED_UCLASS_BODY()
public:
	virtual void Init() override;

	virtual void StartGameInstance() override;
};