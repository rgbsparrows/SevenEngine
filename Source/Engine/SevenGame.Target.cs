using UnrealBuildTool;
using System.Collections.Generic;

[SupportedPlatforms(UnrealPlatformClass.All)]
public class SevenGameTarget : TargetRules
{
	public SevenGameTarget( TargetInfo Target ) : base(Target)
	{
		Type = TargetType.Game;
		BuildEnvironment = TargetBuildEnvironment.Shared;
        CppStandard = CppStandardVersion.Latest;
    }
}
