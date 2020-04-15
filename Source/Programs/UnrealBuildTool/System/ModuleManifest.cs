// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Tools.DotNETCommon;

namespace UnrealBuildTool
{
    /// <summary>
    /// Stores the version (or a unique build ID) for the modules for a target in a certain folder.
    /// 
    /// This allows the runtime to identify which modules are used for which files, and which version they're at. This prevents stale binaries from being
    /// loaded by the runtime when making a local unversioned build, and allows faster incremental builds than compiling the build changelist into every 
    /// module when making versioned builds.
    /// </summary>
    [Serializable]
    class ModuleManifest
    {
        public string BuildId;
        public Dictionary<string, string> ModuleNameToFileName = new Dictionary<string, string>();

        /// <summary>
        /// Constructs the module map with the given changelist
        /// </summary>
        /// <param name="InBuildId">The unique build id</param>
        public ModuleManifest(string InBuildId)
        {
            BuildId = InBuildId;
        }

        /// <summary>
        /// Merge another manifest into this one
        /// </summary>
        /// <param name="Other">The manifest to merge in</param>
        public void Include(ModuleManifest Other)
        {
            foreach (KeyValuePair<string, string> Pair in Other.ModuleNameToFileName)
            {
                if (!ModuleNameToFileName.ContainsKey(Pair.Key))
                {
                    ModuleNameToFileName.Add(Pair.Key, Pair.Value);
                }
            }
        }

        /// <summary>
        /// Gets the standard path for an manifest
        /// </summary>
        /// <param name="AppName">The modular app name being built</param>
        /// <param name="Configuration">The target configuration</param>
        /// <param name="Platform">The target platform</param>
        /// <param name="BuildArchitecture">The architecture of the target platform</param>
        /// <returns>Filename for the app receipt</returns>
        public static string GetStandardFileName(string AppName, UnrealTargetPlatform Platform, UnrealTargetConfiguration Configuration, string BuildArchitecture)
        {
            string BaseName = AppName;
            if (Configuration != UnrealTargetConfiguration.Development)
            {
                BaseName += String.Format("-{0}-{1}", Platform.ToString(), Configuration.ToString());
            }
            if (!String.IsNullOrEmpty(BuildArchitecture) && UEBuildPlatform.GetBuildPlatform(Platform).RequiresArchitectureSuffix())
            {
                BaseName += BuildArchitecture;
            }
            return String.Format("{0}.modules", BaseName);
        }

        /// <summary>
        /// Read an app receipt from disk
        /// </summary>
        /// <param name="FileName">Filename to read from</param>
        /// <returns>The receipt that was read</returns>
        public static ModuleManifest Read(FileReference FileName)
        {
            using (FileStream fileStream = new FileStream(FileName.FullName, FileMode.Open, FileAccess.Write))
            {
                using (BinaryReader reader = new BinaryReader(fileStream))
                {

                    ushort buildIdLength = reader.ReadUInt16();
                    string buildId;

                    buildId = Encoding.Unicode.GetString(reader.ReadBytes(buildIdLength * 2));

                    ModuleManifest Receipt = new ModuleManifest(buildId);

                    ulong moduleCount = reader.ReadUInt64();

                    for(ulong i = 0; i != moduleCount; ++i)
                    {
                        ushort moduleNameLength = reader.ReadUInt16();
                        string moduleName = Encoding.Unicode.GetString(reader.ReadBytes(moduleNameLength * 2));

                        ushort modulePathLength = reader.ReadUInt16();
                        string modulePath = Encoding.Unicode.GetString(reader.ReadBytes(modulePathLength * 2));

                        Receipt.ModuleNameToFileName.Add(moduleName, modulePath);
                    }

                    return Receipt;
                }
            }

        }

        /// <summary>
        /// Tries to read a receipt from disk.
        /// </summary>
        /// <param name="FileName">The filename that was read</param>
        /// <param name="Result">If successful, the receipt that was read. Null otherwise.</param>
        /// <returns>True if the file was read succesfully.</returns>
        public static bool TryRead(FileReference FileName, out ModuleManifest Result)
        {
            if (!FileReference.Exists(FileName))
            {
                Result = null;
                return false;
            }
            try
            {
                Result = Read(FileName);
                return true;
            }
            catch (Exception)
            {
                Result = null;
                return false;
            }
        }

        /// <summary>
        /// Write the receipt to disk.
        /// </summary>
        /// <param name="FileName">The file to write to</param>
        public void Write(string FileName)
        {
            Directory.CreateDirectory(Path.GetDirectoryName(FileName));

            using (FileStream fileStream = new FileStream(FileName, FileMode.Create))
            {
                using (BinaryWriter Writer = new BinaryWriter(fileStream))
                {
                    Write(Writer);
                }
            }
        }

        /// <summary>
        /// Write the receipt to disk.
        /// </summary>
        /// <param name="Writer">The writer to output to</param>
        public void Write(BinaryWriter Writer)
        {
            Writer.Write((ushort)BuildId.Length);
            Writer.Write(Encoding.Unicode.GetBytes(BuildId));

            Writer.Write((ulong)ModuleNameToFileName.Count);

            foreach (KeyValuePair<string, string> ModuleNameToFileNamePair in ModuleNameToFileName.OrderBy(x => x.Key))
            {
                Writer.Write((ushort)ModuleNameToFileNamePair.Key.Length);
                Writer.Write(Encoding.Unicode.GetBytes(ModuleNameToFileNamePair.Key));

                Writer.Write((ushort)ModuleNameToFileNamePair.Value.Length);
                Writer.Write(Encoding.Unicode.GetBytes(ModuleNameToFileNamePair.Value));
            }
        }
    }
}
