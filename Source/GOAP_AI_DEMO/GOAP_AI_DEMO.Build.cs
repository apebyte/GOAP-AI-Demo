// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GOAP_AI_DEMO : ModuleRules
{
	public GOAP_AI_DEMO(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule", "UnrealEd" });
	}
}
