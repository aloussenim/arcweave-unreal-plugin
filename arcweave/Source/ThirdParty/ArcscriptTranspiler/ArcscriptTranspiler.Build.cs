using System;
using System.IO;
using UnrealBuildTool;

public class ArcscriptTranspiler : ModuleRules
{
    public ArcscriptTranspiler(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        bEnableExceptions = true;
        Type = ModuleType.External;

        string ThirdPartyBasePath = Path.Combine(ModuleDirectory, "Source", "ThirdParty", "ArcscriptTranspiler");

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x64", "Release", "antlr4-runtime.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x64", "Release", "ArcscriptTranspiler.lib"));

            PublicDelayLoadDLLs.Add("antlr4-runtime.dll");
            PublicDelayLoadDLLs.Add("ArcscriptTranspiler.dll");
            
            RuntimeDependencies.Add("$(PluginDir)/Source/ThirdParty/ArcscriptTranspiler/x64/Release/antlr4-runtime.dll");
            RuntimeDependencies.Add("$(PluginDir)/Source/ThirdParty/ArcscriptTranspiler/x64/Release/ArcscriptTranspiler.dll");
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            string MacLibSourcePath = Path.Combine(ModuleDirectory, "Mac", "Release");

            string AntlrLibSource = Path.Combine(MacLibSourcePath, "libantlr4-runtime.dylib");
            string TranspilerLibSource = Path.Combine(MacLibSourcePath, "libArcscriptTranspiler.dylib");
            string OutputDir = Path.Combine(PluginDirectory, "Binaries", "Mac");

            PublicAdditionalLibraries.Add(AntlrLibSource);
            PublicAdditionalLibraries.Add(TranspilerLibSource);
            
            PublicDelayLoadDLLs.Add("libantlr4-runtime.dylib");
            PublicDelayLoadDLLs.Add("libArcscriptTranspiler.dylib");
            
            RuntimeDependencies.Add(Path.Combine(OutputDir, "libantlr4-runtime.dylib"), AntlrLibSource);
            RuntimeDependencies.Add(Path.Combine(OutputDir, "ArcscriptTranspiler.dylib"), TranspilerLibSource);

            // Add debug prints to verify the paths UBT is seeing
            System.Console.WriteLine($"DEBUG (ArcscriptTranspiler.build.cs): AntlrLibSource: {AntlrLibSource}");
            System.Console.WriteLine($"DEBUG (ArcscriptTranspiler.build.cs): TranspilerLibSource: {TranspilerLibSource}");
            System.Console.WriteLine($"DEBUG (ArcscriptTranspiler.build.cs): TranspilerLibSource: {OutputDir}");
                        
          PublicSystemIncludePaths.AddRange(
            new string[] {
              MacLibSourcePath,
            }
        );
        }

        PublicIncludePaths.AddRange(new string[]
        {
            Path.Combine(ModuleDirectory, "antlr4-runtime", "src"),
            Path.Combine(ModuleDirectory, "ArcscriptTranspiler", "src"),
            Path.Combine(ModuleDirectory, "ArcscriptTranspiler", "src", "Generated", "ArcscriptParser"),
            Path.Combine(ModuleDirectory, "ArcscriptTranspiler", "src", "Generated", "ArcscriptLexer")
        });
                
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "Projects",
            "CoreUObject",
            "Json",
            "JsonUtilities",
            "Engine",
            "DeveloperSettings",
            "HTTP"
        });
    }
}
