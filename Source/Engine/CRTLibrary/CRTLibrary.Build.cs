using UnrealBuildTool;

public class CRTLibrary : ModuleRules
{
    public CRTLibrary(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.CPlusPlus;

        PublicDependencyModuleNames.Add("Core");
    }
}
