#pragma once
#include "Modules/ModuleManager.h"

class CUSTOMUI_API FCustomUIModule final : public IModuleInterface
{
public:
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;

	static FCustomUIModule& Get();
	
};