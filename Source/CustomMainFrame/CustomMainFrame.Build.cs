// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CustomMainFrame : ModuleRules
{
	public CustomMainFrame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { 
			"CustomEngineExtend",
			"CustomUI",
			"Slate",
			"SlateCore",
			"ApplicationCore",
            "RHI",
            "EngineSettings"
        });
	}
}
