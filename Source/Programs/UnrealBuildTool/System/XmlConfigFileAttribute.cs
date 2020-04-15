// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using System;

namespace UnrealBuildTool
{
    /// <summary>
    /// Marks a field as being serializable from a config file
    /// </summary>
    [AttributeUsage(AttributeTargets.Field, AllowMultiple = true)]
    class XmlConfigFileAttribute : Attribute
    {
        /// <summary>
        /// The category for this config value. Optional; defaults to the declaring type name.
        /// </summary>
        public string Category = null;

        /// <summary>
        /// Name of the key to read. Optional; defaults to the field name.
        /// </summary>
        public string Name = null;
    }
}
