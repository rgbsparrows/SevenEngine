// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool
{
    // This enum has to be compatible with the one defined in the
    // UE4\Engine\Source\Runtime\Core\Public\Misc\ComplilationResult.h 
    // to keep communication between UHT, UBT and Editor compiling
    // processes valid.
    enum ECompilationResult
    {
        /// <summary>
        /// Compilation succeeded
        /// </summary>
        Succeeded = 0,

        /// <summary>
        /// Build was canceled, this is used on the engine side only
        /// </summary>
        Canceled = 1,

        /// <summary>
        /// All targets were up to date, used only with -canskiplink
        /// </summary>
        UpToDate = 2,

        /// <summary>
        /// The process has most likely crashed. This is what UE returns in case of an assert
        /// </summary>
        CrashOrAssert = 3,

        /// <summary>
        /// Compilation failed because generated code changed which was not supported
        /// </summary>
        FailedDueToHeaderChange = 4,

        /// <summary>
        /// Compilation failed due to compilation errors
        /// </summary>
        OtherCompilationError = 5,

        /// <summary>
        /// Compilation is not supported in the current build
        /// </summary>
        Unsupported,

        /// <summary>
        /// Unknown error
        /// </summary>
        Unknown
    }
    static class CompilationResultExtensions
    {
        public static bool Succeeded(this ECompilationResult Result)
        {
            return Result == ECompilationResult.Succeeded || Result == ECompilationResult.UpToDate;
        }
    }

    /// <summary>
    /// This MUST be kept in sync with EGeneratedBodyVersion enum and 
    /// ToGeneratedBodyVersion function in UHT defined in GeneratedCodeVersion.h.
    /// </summary>
    public enum EGeneratedCodeVersion
    {
        /// <summary>
        /// 
        /// </summary>
        None,

        /// <summary>
        /// 
        /// </summary>
        V1,

        /// <summary>
        /// 
        /// </summary>
        V2,

        /// <summary>
        /// 
        /// </summary>
        VLatest = V2
    };
}
