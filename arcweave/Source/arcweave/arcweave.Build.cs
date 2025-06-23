// Copyright Epic Games, Inc. All Rights Reserved.
using System;
using System.IO;
using UnrealBuildTool;

public class arcweave : ModuleRules
{
	public arcweave(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			bUseRTTI = true;
			PublicAdditionalLibraries.Add(Path.Combine(PluginDirectory, "Source", "ThirdParty", "ArcscriptTranspiler", "x64", "Release", "ArcscriptTranspiler.lib"));
			//PublicDelayLoadDLLs.Add("ArcscriptTranspiler.dll");
			RuntimeDependencies.Add("$(PluginDir)/Source/ThirdParty/ArcscriptTranspiler/x64/Release/ArcscriptTranspiler.dll");
		}
		else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
            string MacLibPath = Path.Combine(PluginDirectory, "Source", "ThirdParty", "ArcscriptTranspiler", "Mac", "Release");

            PublicAdditionalLibraries.Add(Path.Combine(MacLibPath, "libArcscriptTranspiler.dylib"));
			RuntimeDependencies.Add("$(PluginDir)/Source/ThirdParty/ArcscriptTranspiler/Mac/Release/libArcscriptTranspiler.dylib");
            PublicDelayLoadDLLs.Add("$(PluginDir)/Source/ThirdParty/ArcscriptTranspiler/Mac/Release/libArcscriptTranspiler.dylib");
		}
		PublicIncludePaths.AddRange(
			new string[] {
				Path.Combine(PluginDirectory, "Source", "ThirdParty", "ArcscriptTranspiler", "ArcscriptTranspiler", "src"),
				// Source/ThirdParty/ArcscriptTranspiler/ArcscriptTranspiler/src/Generated/ArcscriptLexer
				Path.Combine(PluginDirectory, "Source", "ThirdParty", "ArcscriptTranspiler", "ArcscriptTranspiler", "src", "Generated", "ArcscriptParser"),
				Path.Combine(PluginDirectory, "Source", "ThirdParty", "ArcscriptTranspiler", "ArcscriptTranspiler", "src", "Generated", "ArcscriptLexer"),
				Path.Combine(PluginDirectory, "Source", "ThirdParty", "ArcscriptTranspiler", "antlr4-runtime", "src"),
			}
		);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
		);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Projects",
				"Json",
				"JsonUtilities", 
				"Engine",
				"HTTP"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				// ... add private dependencies that you statically link with here ...	
			}
			);
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
		
		PrivateIncludePathModuleNames.AddRange(
			new string[] {
				"Settings" 
			});
	}
}