#include "CustomUI.h"

void FCustomUIModule::StartupModule()
{
}

void FCustomUIModule::ShutdownModule()
{
}

FCustomUIModule& FCustomUIModule::Get()
{
	return FModuleManager::LoadModuleChecked<FCustomUIModule>("CustomUI");
}

IMPLEMENT_MODULE(FCustomUIModule, CustomUI);