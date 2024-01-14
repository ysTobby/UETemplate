#pragma once
#include "Engine/GameEngine.h"
#include "CustomEngine.generated.h"

class SWindow;

UCLASS()
class UCustomEngine : public UGameEngine
{
	GENERATED_UCLASS_BODY()

public:
	virtual void Init(IEngineLoop* InEngineLoop) override;
private:
	TSharedRef<SWindow> CreateCustomWindow();
};