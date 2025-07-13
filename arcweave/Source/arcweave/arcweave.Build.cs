// Copyright Epic Games, Inc. All Rights Reserved.
using System;
using System.IO;
using UnrealBuildTool;

public class arcweave : ModuleRules
{
	public arcweave(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		// Allow c++ exceptions.
        bEnableExceptions = true;
        // Type = ModuleType.External;
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// bUseRTTI = true;
			PublicAdditionalLibraries.Add(Path.Combine(PluginDirectory, "Source", "ThirdParty", "ArcscriptTranspiler", "x64", "Release", "ArcscriptTranspiler.lib"));
			//PublicDelayLoadDLLs.Add("ArcscriptTranspiler.dll");
			RuntimeDependencies.Add("$(PluginDir)/Source/ThirdParty/ArcscriptTranspiler/x64/Release/antlr4-runtime.lib");
			RuntimeDependencies.Add("$(PluginDir)/Source/ThirdParty/ArcscriptTranspiler/x64/Release/ArcscriptTranspiler.dll");
		}
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            string MacLibSourcePath = Path.Combine(PluginDirectory, "Source", "ThirdParty", "ArcscriptTranspiler", "Mac", "Release");

            string AntlrLibSource = Path.Combine(MacLibSourcePath, "libantlr4-runtime.dylib");
            string TranspilerLibSource = Path.Combine(MacLibSourcePath, "libArcscriptTranspiler.dylib");
            string OutputDir = Path.Combine(PluginDirectory, "Binaries", "Mac");

            PublicAdditionalLibraries.Add(AntlrLibSource);
            PublicAdditionalLibraries.Add(TranspilerLibSource);

            RuntimeDependencies.Add(Path.Combine(OutputDir, "libantlr4-runtime.dylib"), AntlrLibSource);
            RuntimeDependencies.Add(Path.Combine(OutputDir, "libArcscriptTranspiler.dylib"), TranspilerLibSource);

            // Add debug prints to verify the paths UBT is seeing
            System.Console.WriteLine($"DEBUG (arcweave.build.cs): AntlrLibSource: {AntlrLibSource}");
            System.Console.WriteLine($"DEBUG (arcweave.build.cs): TranspilerLibSource: {TranspilerLibSource}");
            System.Console.WriteLine($"DEBUG (arcweave.build.cs): OutputDir: {OutputDir}");
            
          PublicSystemLibraryPaths.AddRange(
            new string[] {
              OutputDir,
            }
        );
        }
        
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
		);
			
		PublicIncludePathModuleNames.AddRange(
            new string[]
            {
                "ArcscriptTranspiler"
            });

        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Projects",
				"Json",
				"JsonUtilities",
				"HTTP",
                "Engine"
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
