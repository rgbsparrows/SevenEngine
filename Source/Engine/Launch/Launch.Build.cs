using UnrealBuildTool;
using System.IO;

public class Launch : ModuleRules
{
    public Launch(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.CPlusPlus;

        PrivateDependencyModuleNames.Add("WindowsPlatform");
        PrivateDependencyModuleNames.Add("EngineMain");

    }
}
