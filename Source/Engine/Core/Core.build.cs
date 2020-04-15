using UnrealBuildTool;

public class Core : ModuleRules
{
    public Core(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.CPlusPlus;

        PublicDependencyModuleNames.AddRange(new string[] { "WindowsPlatform", "CRTLibrary" });
    }
}
