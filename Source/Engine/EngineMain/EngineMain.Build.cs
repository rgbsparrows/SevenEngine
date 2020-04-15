using UnrealBuildTool;

public class EngineMain : ModuleRules
{
    public EngineMain(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.CPlusPlus;

        PrivateDependencyModuleNames.AddRange(new string[] { "Core"});
    }
}
