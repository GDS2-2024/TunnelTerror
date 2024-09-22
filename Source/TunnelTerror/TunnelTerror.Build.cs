// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TunnelTerror : ModuleRules
{
	public TunnelTerror(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "AkAudio"});
		PublicDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

	}
}
