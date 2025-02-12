// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class JH_Cropout : ModuleRules
{
	public JH_Cropout(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject", 
			"Engine",
			"InputCore", 
			"EnhancedInput",
			"UMG",
			"CommonUI",
			"Slate",
			"SlateCore" ,
			"GameplayTags",
			"OnlineSubsystem",
			"OnlineSubsystemUtils",
			"AudioModulation",
			"GameplayCameras",
			"IslandGeneratorContent",
			"Niagara",
			"NavigationSystem",
			"AIModule"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] {  "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
