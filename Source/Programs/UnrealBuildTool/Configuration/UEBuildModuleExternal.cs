﻿// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using System.Collections.Generic;
using Tools.DotNETCommon;

namespace UnrealBuildTool
{
    /// <summary>
    /// A module that is never compiled by us, and is only used to group include paths and libraries into a dependency unit.
    /// </summary>
    class UEBuildModuleExternal : UEBuildModule
    {
        public UEBuildModuleExternal(
            string InName,
            string InPackageName,
            DirectoryReference InModuleDirectory,
            ModuleRules InRules,
            FileReference InRulesFile,
            List<RuntimeDependency> InRuntimeDependencies
            )
            : base(
                InName: InName,
                InPackageName: InPackageName,
                InModuleDirectory: InModuleDirectory,
                InRules: InRules,
                InRulesFile: InRulesFile,
                InRuntimeDependencies: InRuntimeDependencies
                )
        {
        }

        // UEBuildModule interface.
        public override List<FileItem> Compile(ReadOnlyTargetRules Target, UEToolChain ToolChain, CppCompileEnvironment CompileEnvironment, List<PrecompiledHeaderTemplate> SharedPCHs, ISourceFileWorkingSet WorkingSet, ActionGraph ActionGraph)
        {
            return new List<FileItem>();
        }
    }
}
