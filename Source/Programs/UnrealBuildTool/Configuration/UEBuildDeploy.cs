﻿// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool
{
    /// <summary>
    /// Base class to handle deploy of a target for a given platform
    /// </summary>
    abstract class UEBuildDeploy
    {
        /// <summary>
        /// Prepare the target for deployment
        /// </summary>
        /// <param name="InTarget"> The target for deployment</param>
        /// <returns>bool   true if successful, false if not</returns>
        public virtual bool PrepTargetForDeployment(UEBuildDeployTarget InTarget)
        {
            return true;
        }
    }
}
