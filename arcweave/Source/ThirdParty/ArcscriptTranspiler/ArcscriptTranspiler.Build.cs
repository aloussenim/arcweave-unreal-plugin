using System;
using System.IO;
using UnrealBuildTool;
public class ArcscriptTranspiler : ModuleRules
{
    public ArcscriptTranspiler(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.External;

        string ThirdPartyBasePath = Path.Combine(ModuleDirectory, "Source", "ThirdParty", "ArcscriptTranspiler");

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            // Add the import library
            PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyBasePath, "x64", "Release", "antlr4-runtime.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyBasePath, "x64", "Release", "ArcscriptTranspiler.lib"));

            // Delay-load the DLL, so we can load it from the right place first
            PublicDelayLoadDLLs.Add("antlr4-runtime.dll");
            PublicDelayLoadDLLs.Add("ArcscriptTranspiler.dll");
            // Ensure that the DLL is staged along with the executable
            RuntimeDependencies.Add(Path.Combine(ThirdPartyBasePath, "x64", "Release", "antlr4-runtime.dll"));
            RuntimeDependencies.Add(Path.Combine(ThirdPartyBasePath, "x64", "Release", "ArcscriptTranspiler.dll"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            // Ensure that the DLL is staged along with the executable
            // Use Path.Combine with ModuleDirectory or ThirdPartyBasePath
            RuntimeDependencies.Add(Path.Combine(ThirdPartyBasePath, "Mac", "Release", "libantlr4-runtime.dylib"));
            RuntimeDependencies.Add(Path.Combine(ThirdPartyBasePath, "Mac", "Release", "libArcscriptTranspiler.dylib"));
        }

        // PublicIncludePaths should also use Path.Combine with the ModuleDirectory or ThirdPartyBasePath
        PublicIncludePaths.AddRange(new string[]
        {
            Path.Combine(ThirdPartyBasePath, "antlr4-runtime", "src"),
            Path.Combine(ThirdPartyBasePath, "ArcscriptTranspiler", "src"),
            Path.Combine(ThirdPartyBasePath, "ArcscriptTranspiler", "src", "Generated", "ArcscriptParser"),
            Path.Combine(ThirdPartyBasePath, "ArcscriptTranspiler", "src", "Generated", "ArcscriptLexer")
        });

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "Projects",
            "CoreUObject",
            "Json",
            "JsonUtilities",
            "Engine",
            "DeveloperSettings"
        });
    }
}