using UnrealBuildTool;

public class WindowsPlatform : ModuleRules
{
    public WindowsPlatform(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.CPlusPlus;

        PublicDependencyModuleNames.Add("Core");

        //避免windows宏对引擎代码的影响，需要强制关闭unity编译
        bFasterWithoutUnity = true;
    }
}
