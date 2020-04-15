// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Tools.DotNETCommon;

namespace UnrealBuildTool
{
    [Serializable]
    class SerializedPluginDescriptor
    {
        /// <summary>
        /// Full name for directory of the plugin
        /// </summary>
        public string DirectoryFullName;

        /// <summary>
        /// Descriptor version number
        /// </summary>
        public int FileVersion;

        /// <summary>
        /// Version number for the plugin.  The version number must increase with every version of the plugin, so that the system 
        /// can determine whether one version of a plugin is newer than another, or to enforce other requirements.  This version
        /// number is not displayed in front-facing UI.  Use the VersionName for that.
        /// </summary>
        public int Version;

        /// <summary>
        /// Name of the version for this plugin.  This is the front-facing part of the version number.  It doesn't need to match
        /// the version number numerically, but should be updated when the version number is increased accordingly.
        /// </summary>
        public string VersionName;

        /// <summary>
        /// Friendly name of the plugin
        /// </summary>
        public string FriendlyName;

        /// <summary>
        /// Description of the plugin
        /// </summary>
        public string Description;

        /// <summary>
        /// The name of the category this plugin
        /// </summary>
        public string Category;

        /// <summary>
        /// The company or individual who created this plugin.  This is an optional field that may be displayed in the user interface.
        /// </summary>
        public string CreatedBy;

        /// <summary>
        /// Hyperlink URL string for the company or individual who created this plugin.  This is optional.
        /// </summary>
        public string CreatedByURL;

        /// <summary>
        /// Documentation URL string.
        /// </summary>
        public string DocsURL;

        /// <summary>
        /// Marketplace URL for this plugin. This URL will be embedded into projects that enable this plugin, so we can redirect to the marketplace if a user doesn't have it installed.
        /// </summary>
        public string MarketplaceURL;

        /// <summary>
        /// Support URL/email for this plugin.
        /// </summary>
        public string SupportURL;

        /// <summary>
        /// Sets the version of the engine that this plugin is compatible with.
        /// </summary>
        public string EngineVersion;

        private SerializedPluginDescriptor() { }

        public SerializedPluginDescriptor(FileReference pluginFile, DirectoryReference projectDirectory)
        {
            PluginDescriptor pluginDescriptor = PluginDescriptor.FromFile(pluginFile);

            DirectoryFullName = pluginFile.Directory.MakeRelativeTo(projectDirectory);

            FileVersion = pluginDescriptor.FileVersion;
            Version = pluginDescriptor.Version;
            VersionName = pluginDescriptor.VersionName;
            FriendlyName = pluginDescriptor.FriendlyName;
            Description = pluginDescriptor.Description;
            Category = pluginDescriptor.Category;
            CreatedBy = pluginDescriptor.CreatedBy;
            CreatedByURL = pluginDescriptor.CreatedByURL;
            DocsURL = pluginDescriptor.DocsURL;
            MarketplaceURL = pluginDescriptor.MarketplaceURL;
            SupportURL = pluginDescriptor.SupportURL;
            EngineVersion = pluginDescriptor.EngineVersion;
        }

        public static SerializedPluginDescriptor Read(BinaryReader binaryReader)
        {
            SerializedPluginDescriptor desc = new SerializedPluginDescriptor();

            UInt16 DirectoryFullNameLength = binaryReader.ReadUInt16();
            desc.DirectoryFullName = Encoding.Unicode.GetString(binaryReader.ReadBytes(DirectoryFullNameLength * 2));

            desc.FileVersion = binaryReader.ReadInt32();
            desc.Version = binaryReader.ReadInt32();

            UInt16 VersionNameLength = binaryReader.ReadUInt16();
            desc.VersionName = Encoding.Unicode.GetString(binaryReader.ReadBytes(VersionNameLength * 2));

            UInt16 FriendlyNameLength = binaryReader.ReadUInt16();
            desc.FriendlyName = Encoding.Unicode.GetString(binaryReader.ReadBytes(FriendlyNameLength * 2));

            UInt16 DescriptionLength = binaryReader.ReadUInt16();
            desc.Description = Encoding.Unicode.GetString(binaryReader.ReadBytes(DescriptionLength * 2));

            UInt16 CategoryLength = binaryReader.ReadUInt16();
            desc.Category = Encoding.Unicode.GetString(binaryReader.ReadBytes(CategoryLength * 2));

            UInt16 CreatedByLength = binaryReader.ReadUInt16();
            desc.CreatedBy = Encoding.Unicode.GetString(binaryReader.ReadBytes(CreatedByLength * 2));

            UInt16 CreatedByURLLength = binaryReader.ReadUInt16();
            desc.CreatedByURL = Encoding.Unicode.GetString(binaryReader.ReadBytes(CreatedByURLLength * 2));

            UInt16 DocsURLLength = binaryReader.ReadUInt16();
            desc.DocsURL = Encoding.Unicode.GetString(binaryReader.ReadBytes(DocsURLLength * 2));

            UInt16 MarketplaceURLLength = binaryReader.ReadUInt16();
            desc.MarketplaceURL = Encoding.Unicode.GetString(binaryReader.ReadBytes(MarketplaceURLLength * 2));

            UInt16 SupportURLLength = binaryReader.ReadUInt16();
            desc.SupportURL = Encoding.Unicode.GetString(binaryReader.ReadBytes(SupportURLLength * 2));

            UInt16 EngineVersionLength = binaryReader.ReadUInt16();
            desc.EngineVersion = Encoding.Unicode.GetString(binaryReader.ReadBytes(EngineVersionLength * 2));

            return desc;
        }

        public void Write(BinaryWriter binaryWriter)
        {
            binaryWriter.Write((ushort)DirectoryFullName.Length);
            binaryWriter.Write(Encoding.Unicode.GetBytes(DirectoryFullName));

            binaryWriter.Write(FileVersion);
            binaryWriter.Write(Version);

            binaryWriter.Write((ushort)VersionName.Length);
            binaryWriter.Write(Encoding.Unicode.GetBytes(VersionName));

            binaryWriter.Write((ushort)FriendlyName.Length);
            binaryWriter.Write(Encoding.Unicode.GetBytes(FriendlyName));

            binaryWriter.Write((ushort)Description.Length);
            binaryWriter.Write(Encoding.Unicode.GetBytes(Description));

            binaryWriter.Write((ushort)Category.Length);
            binaryWriter.Write(Encoding.Unicode.GetBytes(Category));

            binaryWriter.Write((ushort)CreatedBy.Length);
            binaryWriter.Write(Encoding.Unicode.GetBytes(CreatedBy));

            binaryWriter.Write((ushort)CreatedByURL.Length);
            binaryWriter.Write(Encoding.Unicode.GetBytes(CreatedByURL));

            binaryWriter.Write((ushort)DocsURL.Length);
            binaryWriter.Write(Encoding.Unicode.GetBytes(DocsURL));

            binaryWriter.Write((ushort)MarketplaceURL.Length);
            binaryWriter.Write(Encoding.Unicode.GetBytes(MarketplaceURL));

            binaryWriter.Write((ushort)SupportURL.Length);
            binaryWriter.Write(Encoding.Unicode.GetBytes(SupportURL));

            binaryWriter.Write((ushort)EngineVersion.Length);
            binaryWriter.Write(Encoding.Unicode.GetBytes(EngineVersion));
        }
    }

    /// <summary>
    /// Stores the plugins for a target in a certain folder.
    /// </summary>
    [Serializable]
    class PluginManifest
    {
        public List<SerializedPluginDescriptor> PluginDescriptors = new List<SerializedPluginDescriptor>();

        /// <summary>
        /// Constructs the module map with the given changelist
        /// </summary>
        /// <param name="projectDirectory">The project directory</param>
        public PluginManifest(DirectoryReference projectDirectory)
        {
            DirectoryReference ProjectPluginsDir = DirectoryReference.Combine(projectDirectory, "Plugins");
            foreach (FileReference pluginFile in Plugins.EnumeratePlugins(ProjectPluginsDir))
            {
                PluginDescriptors.Add(new SerializedPluginDescriptor(pluginFile, ProjectPluginsDir));
            }
        }

        private PluginManifest() { }

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
            return String.Format("{0}.plugins", BaseName);
        }

        /// <summary>
        /// Read an app receipt from disk
        /// </summary>
        /// <param name="FileName">Filename to read from</param>
        /// <returns>The receipt that was read</returns>
        public static PluginManifest Read(FileReference FileName)
        {
            using (FileStream fileStream = new FileStream(FileName.FullName, FileMode.Open, FileAccess.Read))
            {
                using (BinaryReader reader = new BinaryReader(fileStream))
                {
                    PluginManifest pluginManifest = new PluginManifest();

                    ulong pluginCount = reader.ReadUInt64();

                    for (ulong i = 0; i != pluginCount; ++i)
                    {
                        pluginManifest.PluginDescriptors.Add(SerializedPluginDescriptor.Read(reader));
                    }

                    return pluginManifest;
                }
            }
        }

        /// <summary>
        /// Tries to read a manifest from disk.
        /// </summary>
        /// <param name="FileName">The filename that was read</param>
        /// <param name="Result">If successful, the receipt that was read. Null otherwise.</param>
        /// <returns>True if the file was read succesfully.</returns>
        public static bool TryRead(FileReference FileName, out PluginManifest Result)
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
        /// Write the manifest to disk.
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
        /// Write the manifest to disk.
        /// </summary>
        /// <param name="Writer">The writer to output to</param>
        public void Write(BinaryWriter Writer)
        {
            Writer.Write((ulong)PluginDescriptors.Count);

            foreach (SerializedPluginDescriptor pluginDesc in PluginDescriptors.OrderBy(x => x.FriendlyName))
            {
                pluginDesc.Write(Writer);
            }
        }
    }
}
