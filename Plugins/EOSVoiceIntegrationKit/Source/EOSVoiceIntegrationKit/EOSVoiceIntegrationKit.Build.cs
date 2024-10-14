// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;

namespace UnrealBuildTool.Rules
{
	public class EOSVoiceIntegrationKit : ModuleRules
	{
		public EOSVoiceIntegrationKit(ReadOnlyTargetRules Target) : base(Target)
		{
			PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

			PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public"));

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Json"
				}
			);

			PublicIncludePathModuleNames.AddRange(
				new string[]
				{
					"VoiceChat",
				}
			);

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
					"Projects",
					"EOSShared",
					"EOSSDK",
					//"EOSVoiceIntegrationKit",
					"Engine",
					"StructUtils",
					"HTTP",
				}
			);

			if(Target.Platform == UnrealTargetPlatform.IOS)
			{
				PrivateDependencyModuleNames.AddRange(
					new string[]
					{
						"ApplicationCore"
					}
				);
			}
		}
	}
}
