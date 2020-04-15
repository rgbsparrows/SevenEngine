using UnrealBuildTool;

public class WindowsPlatform : ModuleRules
{
    public WindowsPlatform(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.CPlusPlus;

        PublicDependencyModuleNames.Add("Core");

        //����windows�����������Ӱ�죬��Ҫǿ�ƹر�unity����
        bFasterWithoutUnity = true;
    }
}
