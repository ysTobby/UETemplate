// Copyright Epic Games, Inc. All Rights Reserved.

#include "Modules/ModuleManager.h"

class FCustomMainFrameModule final : public IModuleInterface
{
};

IMPLEMENT_PRIMARY_GAME_MODULE(FCustomMainFrameModule, CustomMainFrame, "CustomMainFrame");
