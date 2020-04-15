// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using Tools.DotNETCommon;

namespace UnrealBuildTool
{
    class VCToolChain : UEToolChain
    {
        /// <summary>
        /// The target being built
        /// </summary>
        protected ReadOnlyTargetRules Target;

        /// <summary>
        /// The Visual C++ environment
        /// </summary>
        protected VCEnvironment EnvVars;

        public VCToolChain(CppPlatform Platform, ReadOnlyTargetRules Target)
            : base(Platform)
        {
            this.Target = Target;
            this.EnvVars = VCEnvironment.Create(Target.WindowsPlatform.Compiler, Platform, Target.WindowsPlatform.CompilerVersion, Target.WindowsPlatform.WindowsSdkVersion);

            if (Target.WindowsPlatform.ObjSrcMapFile != null)
            {
                try
                {
                    File.Delete(Target.WindowsPlatform.ObjSrcMapFile);
                }
                catch
                {
                }
            }
        }

        public override void PrintVersionInfo()
        {
            Log.TraceInformation("Using {0} {1} toolchain ({2}) and Windows {3} SDK ({4}).", WindowsPlatform.GetCompilerName(Target.WindowsPlatform.Compiler), EnvVars.ToolChainVersion, EnvVars.ToolChainDir, EnvVars.WindowsSdkVersion, EnvVars.WindowsSdkDir);
            Log.TraceLog("Using {0} compiler toolchain in {1}", EnvVars.ToolChainVersion, EnvVars.ToolChainDir);
            Log.TraceLog("Using Windows SDK {0} in {1}", EnvVars.WindowsSdkVersion, EnvVars.WindowsSdkDir);
        }

        static void AddDefinition(List<string> Arguments, string Definition)
        {
            // Split the definition into name and value
            int ValueIdx = Definition.IndexOf('=');
            if (ValueIdx == -1)
            {
                AddDefinition(Arguments, Definition, null);
            }
            else
            {
                AddDefinition(Arguments, Definition.Substring(0, ValueIdx), Definition.Substring(ValueIdx + 1));
            }
        }


        static void AddDefinition(List<string> Arguments, string Variable, string Value)
        {
            // If the value has a space in it and isn't wrapped in quotes, do that now
            if (Value != null && !Value.StartsWith("\"") && (Value.Contains(" ") || Value.Contains("$")))
            {
                Value = "\"" + Value + "\"";
            }

            if (Value != null)
            {
                Arguments.Add("/D" + Variable + "=" + Value);
            }
            else
            {
                Arguments.Add("/D" + Variable);
            }
        }


        static void AddIncludePath(List<string> Arguments, DirectoryReference IncludePath)
        {
            // If the value has a space in it and isn't wrapped in quotes, do that now. Make sure it doesn't include a trailing slash, because that will escape the closing quote.
            string IncludePathString;
            if (IncludePath.IsUnderDirectory(UnrealBuildTool.RootDirectory))
            {
                IncludePathString = IncludePath.MakeRelativeTo(UnrealBuildTool.EngineSourceDirectory);
            }
            else
            {
                IncludePathString = IncludePath.FullName;
            }

            if (IncludePathString.Contains(" "))
            {
                IncludePathString = "\"" + IncludePathString + "\"";
            }

            Arguments.Add("/I " + IncludePathString);
        }

        static void AddSystemIncludePath(List<string> Arguments, DirectoryReference IncludePath)
        {
            AddIncludePath(Arguments, IncludePath);
        }


        void AppendCLArguments_Global(CppCompileEnvironment CompileEnvironment, List<string> Arguments)
        {
            // Suppress generation of object code for unreferenced inline functions. Enabling this option is more standards compliant, and causes a big reduction
            // in object file sizes (and link times) due to the amount of stuff we inline.
            if (Target.WindowsPlatform.Compiler >= WindowsCompiler.VisualStudio2015)
            {
                Arguments.Add("/Zc:inline");
            }

            if (Target.WindowsPlatform.StaticAnalyzer == WindowsStaticAnalyzer.VisualCpp)
            {
                Arguments.Add("/analyze");

                // Don't cause analyze warnings to be errors
                Arguments.Add("/analyze:WX-");

                // Report functions that use a LOT of stack space.  You can lower this value if you
                // want more aggressive checking for functions that use a lot of stack memory.
                Arguments.Add("/analyze:stacksize81940");

                // Don't bother generating code, only analyze code (may report fewer warnings though.)
                //Arguments.Add("/analyze:only");
            }

            // Prevents the compiler from displaying its logo for each invocation.
            Arguments.Add("/nologo");

            // Enable intrinsic functions.
            Arguments.Add("/Oi");

            // Compile into an .obj file, and skip linking.
            Arguments.Add("/c");

            // Separate functions for linker.
            Arguments.Add("/Gy");

            // Allow 750% of the default memory allocation limit when using the static analyzer, and 850% at other times.
            if (Target.WindowsPlatform.StaticAnalyzer == WindowsStaticAnalyzer.VisualCpp)
            {
                Arguments.Add("/Zm750");
            }
            else
            {
                Arguments.Add("/Zm850");
            }

            // Disable "The file contains a character that cannot be represented in the current code page" warning for non-US windows.
            Arguments.Add("/wd4819");

            // Disable Microsoft extensions on VS2017+ for improved standards compliance.
            if (Target.WindowsPlatform.Compiler >= WindowsCompiler.VisualStudio2017 && Target.WindowsPlatform.bStrictConformanceMode)
            {
                Arguments.Add("/permissive-");
                Arguments.Add("/Zc:strictStrings-"); // Have to disable strict const char* semantics due to Windows headers not being compliant.
            }

            // @todo UWP: UE4 is non-compliant when it comes to use of %s and %S
            // Previously %s meant "the current character set" and %S meant "the other one".
            // Now %s means multibyte and %S means wide. %Ts means "natural width".
            // Reverting this behaviour until the UE4 source catches up.
            if (Target.WindowsPlatform.Compiler >= WindowsCompiler.VisualStudio2015)
            {
                AddDefinition(Arguments, "_CRT_STDIO_LEGACY_WIDE_SPECIFIERS=1");
            }

            // @todo UWP: Silence the hash_map deprecation errors for now. This should be replaced with unordered_map for the real fix.
            if (Target.WindowsPlatform.Compiler >= WindowsCompiler.VisualStudio2015)
            {
                AddDefinition(Arguments, "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS=1");
            }

            // If compiling as a DLL, set the relevant defines
            if (CompileEnvironment.bIsBuildingDLL)
            {
                AddDefinition(Arguments, "_WINDLL");
            }

            // Fix Incredibuild errors with helpers using heterogeneous character sets
            if (Target.WindowsPlatform.Compiler >= WindowsCompiler.VisualStudio2015)
            {
                Arguments.Add("/source-charset:utf-8 /execution-charset:utf-8");
            }

            //
            //	Debug
            //
            if (CompileEnvironment.Configuration == CppConfiguration.Debug)
            {
                // Disable compiler optimization.
                Arguments.Add("/Od");

                // Favor code size (especially useful for embedded platforms).
                Arguments.Add("/Os");

                // Allow inline method expansion unless E&C support is requested
                if (!CompileEnvironment.bSupportEditAndContinue && CompileEnvironment.bUseInlining)
                {
                    Arguments.Add("/Ob2");
                }

                if ((CompileEnvironment.Platform == CppPlatform.Win32) ||
                    (CompileEnvironment.Platform == CppPlatform.Win64))
                {
                    Arguments.Add("/RTCs");
                }
            }
            //
            //	Development and LTCG
            //
            else
            {
                if (!CompileEnvironment.bOptimizeCode)
                {
                    // Disable compiler optimization.
                    Arguments.Add("/Od");
                }
                else
                {
                    // Maximum optimizations.
                    Arguments.Add("/Ox");

                    // Favor code speed.
                    Arguments.Add("/Ot");

                    // Only omit frame pointers on the PC (which is implied by /Ox) if wanted.
                    if (CompileEnvironment.bOmitFramePointers == false
                    && ((CompileEnvironment.Platform == CppPlatform.Win32) ||
                        (CompileEnvironment.Platform == CppPlatform.Win64)))
                    {
                        Arguments.Add("/Oy-");
                    }
                }

                // Allow inline method expansion
                Arguments.Add("/Ob2");

                //
                // LTCG
                //
                if (CompileEnvironment.bAllowLTCG)
                {
                    // Enable link-time code generation.
                    Arguments.Add("/GL");
                }
            }

            //
            //	PC
            //
            if ((CompileEnvironment.Platform == CppPlatform.Win32) ||
                (CompileEnvironment.Platform == CppPlatform.Win64))
            {
                if (CompileEnvironment.bUseAVX)
                {
                    // Allow the compiler to generate AVX instructions.
                    Arguments.Add("/arch:AVX");
                }
                // SSE options are not allowed when using the 64 bit toolchain
                // (enables SSE2 automatically)
                else if (CompileEnvironment.Platform != CppPlatform.Win64)
                {
                    // Allow the compiler to generate SSE2 instructions.
                    Arguments.Add("/arch:SSE2");
                }

                // Prompt the user before reporting internal errors to Microsoft.
                Arguments.Add("/errorReport:prompt");


                // Enable C++ exceptions when building with the editor or when building UHT.
                if ((CompileEnvironment.bEnableExceptions))
                {
                    // Enable C++ exception handling, but not C exceptions.
                    Arguments.Add("/EHsc");
                }
                else
                {
                    // This is required to disable exception handling in VC platform headers.
                    CompileEnvironment.Definitions.Add("_HAS_EXCEPTIONS=0");
                }
            }

            // If enabled, create debug information.
            if (CompileEnvironment.bCreateDebugInfo)
            {
                // Store debug info in .pdb files.
                if (CompileEnvironment.bUsePDBFiles)
                {
                    // Create debug info suitable for E&C if wanted.
                    if (CompileEnvironment.bSupportEditAndContinue &&
                        // We only need to do this in debug as that's the only configuration that supports E&C.
                        CompileEnvironment.Configuration == CppConfiguration.Debug)
                    {
                        Arguments.Add("/ZI");
                    }
                    // Regular PDB debug information.
                    else
                    {
                        Arguments.Add("/Zi");
                    }
                    // We need to add this so VS won't lock the PDB file and prevent synchronous updates. This forces serialization through MSPDBSRV.exe.
                    // See http://msdn.microsoft.com/en-us/library/dn502518.aspx for deeper discussion of /FS switch.
                    if (CompileEnvironment.bUseIncrementalLinking)
                    {
                        Arguments.Add("/FS");
                    }
                }
                // Store C7-format debug info in the .obj files, which is faster.
                else
                {
                    Arguments.Add("/Z7");
                }
            }

            // Specify the appropriate runtime library based on the platform and config.
            if (CompileEnvironment.bUseStaticCRT)
            {
                if (CompileEnvironment.bUseDebugCRT)
                {
                    Arguments.Add("/MTd");

                }
                else
                {
                    Arguments.Add("/MT");
                }
            }
            else
            {
                if (CompileEnvironment.bUseDebugCRT)
                {
                    Arguments.Add("/MDd");

                }
                else
                {
                    Arguments.Add("/MD");
                }
            }

            if (CompileEnvironment.bOptimizeCode)
            {
                // Allow optimized code to be debugged more easily.  This makes PDBs a bit larger, but doesn't noticeably affect
                // compile times.  The executable code is not affected at all by this switch, only the debugging information.
                Arguments.Add("/Zo");
            }

            if (CompileEnvironment.Platform == CppPlatform.Win64)
            {
                // Pack struct members on 8-byte boundaries.
                Arguments.Add("/Zp8");
            }
            else
            {
                Arguments.Add("/Zp4");
            }

            //@todo: Disable warnings for VS2015. These should be reenabled as we clear the reasons for them out of the engine source and the VS2015 toolchain evolves.
            if (Target.WindowsPlatform.Compiler >= WindowsCompiler.VisualStudio2015)
            {
                // Disable shadow variable warnings
                if (CompileEnvironment.bEnableShadowVariableWarnings == false)
                {
                    Arguments.Add("/wd4456"); // 4456 - declaration of 'LocalVariable' hides previous local declaration
                    Arguments.Add("/wd4458"); // 4458 - declaration of 'parameter' hides class member
                    Arguments.Add("/wd4459"); // 4459 - declaration of 'LocalVariable' hides global declaration
                }

                Arguments.Add("/wd4463"); // 4463 - overflow; assigning 1 to bit-field that can only hold values from -1 to 0

                Arguments.Add("/wd4838"); // 4838: conversion from 'type1' to 'type2' requires a narrowing conversion
            }

            if (CompileEnvironment.bEnableUndefinedIdentifierWarnings)
            {
                if (CompileEnvironment.bUndefinedIdentifierWarningsAsErrors)
                {
                    Arguments.Add("/we4668");
                }
                else
                {
                    Arguments.Add("/w44668");
                }
            }
        }

        static void AppendCLArguments_CPP(CppCompileEnvironment CompileEnvironment, List<string> Arguments)
        {
            // Explicitly compile the file as C++.
            Arguments.Add("/TP");

            if (!CompileEnvironment.bEnableBufferSecurityChecks)
            {
                // This will disable buffer security checks (which are enabled by default) that the MS compiler adds around arrays on the stack,
                // Which can add some performance overhead, especially in performance intensive code
                // Only disable this if you know what you are doing, because it will be disabled for the entire module!
                Arguments.Add("/GS-");
            }

            // Configure RTTI
            if (CompileEnvironment.bUseRTTI)
            {
                // Enable C++ RTTI.
                Arguments.Add("/GR");

            }
            else
            {
                // Disable C++ RTTI.
                Arguments.Add("/GR-");
            }

            // Set warning level.
            Arguments.Add("/W4");

            if (CompileEnvironment.CppStandard >= CppStandardVersion.Latest)
            {
                Arguments.Add("/std:c++latest");
            }
            else if (CompileEnvironment.CppStandard >= CppStandardVersion.Cpp17)
            {
                Arguments.Add("/std:c++17");
            }
            else if (CompileEnvironment.CppStandard >= CppStandardVersion.Cpp14)
            {
                Arguments.Add("/std:c++14");
            }
        }

        static void AppendCLArguments_C(List<string> Arguments)
        {
            // Explicitly compile the file as C.
            Arguments.Add("/TC");

            // Level 0 warnings.  Needed for external C projects that produce warnings at higher warning levels.
            Arguments.Add("/W0");
        }

        void AppendLinkArguments(LinkEnvironment LinkEnvironment, List<string> Arguments)
        {
            // Don't create a side-by-side manifest file for the executable.
            Arguments.Add("/MANIFEST:NO");

            // Prevents the linker from displaying its logo for each invocation.
            Arguments.Add("/NOLOGO");

            if (LinkEnvironment.bCreateDebugInfo)
            {
                // Output debug info for the linked executable.
                Arguments.Add("/DEBUG");

                // Allow partial PDBs for faster linking
                if (Target.WindowsPlatform.Compiler >= WindowsCompiler.VisualStudio2015 && LinkEnvironment.bUseFastPDBLinking)
                {
                    Arguments[Arguments.Count - 1] += ":FASTLINK";
                }
            }

            // Prompt the user before reporting internal errors to Microsoft.
            Arguments.Add("/errorReport:prompt");

            //
            //	PC
            //
            if ((LinkEnvironment.Platform == CppPlatform.Win32) ||
                (LinkEnvironment.Platform == CppPlatform.Win64))
            {
                // Set machine type/ architecture to be 64 bit.
                if (LinkEnvironment.Platform == CppPlatform.Win64)
                {
                    Arguments.Add("/MACHINE:x64");
                }
                // 32 bit executable/ target.
                else
                {
                    Arguments.Add("/MACHINE:x86");
                }

                {
                    if (LinkEnvironment.bIsBuildingConsoleApplication)
                    {
                        Arguments.Add("/SUBSYSTEM:CONSOLE");
                    }
                    else
                    {
                        Arguments.Add("/SUBSYSTEM:WINDOWS");
                    }
                }

                if (LinkEnvironment.bIsBuildingConsoleApplication && !LinkEnvironment.bIsBuildingDLL && !String.IsNullOrEmpty(LinkEnvironment.WindowsEntryPointOverride))
                {
                    // Use overridden entry point
                    Arguments.Add("/ENTRY:" + LinkEnvironment.WindowsEntryPointOverride);
                }

                // Allow the OS to load the EXE at different base addresses than its preferred base address.
                Arguments.Add("/FIXED:No");

                // Option is only relevant with 32 bit toolchain.
                if ((LinkEnvironment.Platform == CppPlatform.Win32) && WindowsPlatform.bBuildLargeAddressAwareBinary)
                {
                    // Disables the 2GB address space limit on 64-bit Windows and 32-bit Windows with /3GB specified in boot.ini
                    Arguments.Add("/LARGEADDRESSAWARE");
                }

                // Explicitly declare that the executable is compatible with Data Execution Prevention.
                Arguments.Add("/NXCOMPAT");

                // Set the default stack size.
                if (LinkEnvironment.DefaultStackSizeCommit > 0)
                {
                    Arguments.Add("/STACK:" + LinkEnvironment.DefaultStackSize + "," + LinkEnvironment.DefaultStackSizeCommit);
                }
                else
                {
                    Arguments.Add("/STACK:" + LinkEnvironment.DefaultStackSize);
                }

                // E&C can't use /SAFESEH.  Also, /SAFESEH isn't compatible with 64-bit linking
                if (!LinkEnvironment.bSupportEditAndContinue &&
                    LinkEnvironment.Platform != CppPlatform.Win64)
                {
                    // Generates a table of Safe Exception Handlers.  Documentation isn't clear whether they actually mean
                    // Structured Exception Handlers.
                    Arguments.Add("/SAFESEH");
                }

                // Allow delay-loaded DLLs to be explicitly unloaded.
                Arguments.Add("/DELAY:UNLOAD");

                if (LinkEnvironment.bIsBuildingDLL)
                {
                    Arguments.Add("/DLL");
                }
            }

            // Don't embed the full PDB path; we want to be able to move binaries elsewhere. They will always be side by side.
            Arguments.Add("/PDBALTPATH:%_PDB%");

            //
            //	Shipping & LTCG
            //
            if (LinkEnvironment.bAllowLTCG)
            {
                // Use link-time code generation.
                Arguments.Add("/LTCG");

                // This is where we add in the PGO-Lite linkorder.txt if we are using PGO-Lite
                //Result += " /ORDER:@linkorder.txt";
                //Result += " /VERBOSE";
            }

            //
            //	Shipping binary
            //
            if (LinkEnvironment.Configuration == CppConfiguration.Shipping)
            {
                // Generate an EXE checksum.
                Arguments.Add("/RELEASE");

                // Eliminate unreferenced symbols.
                Arguments.Add("/OPT:REF");

                // Remove redundant COMDATs.
                Arguments.Add("/OPT:ICF");
            }
            //
            //	Regular development binary. 
            //
            else
            {
                // Keep symbols that are unreferenced.
                Arguments.Add("/OPT:NOREF");

                // Disable identical COMDAT folding.
                Arguments.Add("/OPT:NOICF");
            }

            // Enable incremental linking if wanted.
            if (LinkEnvironment.bUseIncrementalLinking)
            {
                Arguments.Add("/INCREMENTAL");
            }
            else
            {
                Arguments.Add("/INCREMENTAL:NO");
            }

            // Disable 
            //LINK : warning LNK4199: /DELAYLOAD:nvtt_64.dll ignored; no imports found from nvtt_64.dll
            // type warning as we leverage the DelayLoad option to put third-party DLLs into a 
            // non-standard location. This requires the module(s) that use said DLL to ensure that it 
            // is loaded prior to using it.
            Arguments.Add("/ignore:4199");

            // Suppress warnings about missing PDB files for statically linked libraries.  We often don't want to distribute
            // PDB files for these libraries.
            Arguments.Add("/ignore:4099");      // warning LNK4099: PDB '<file>' was not found with '<file>'
        }

        void AppendLibArguments(LinkEnvironment LinkEnvironment, List<string> Arguments)
        {
            // Prevents the linker from displaying its logo for each invocation.
            Arguments.Add("/NOLOGO");

            // Prompt the user before reporting internal errors to Microsoft.
            Arguments.Add("/errorReport:prompt");

            //
            //	PC
            //
            if (LinkEnvironment.Platform == CppPlatform.Win32 || LinkEnvironment.Platform == CppPlatform.Win64)
            {
                // Set machine type/ architecture to be 64 bit.
                if (LinkEnvironment.Platform == CppPlatform.Win64)
                {
                    Arguments.Add("/MACHINE:x64");
                }
                // 32 bit executable/ target.
                else
                {
                    Arguments.Add("/MACHINE:x86");
                }

                {
                    if (LinkEnvironment.bIsBuildingConsoleApplication)
                    {
                        Arguments.Add("/SUBSYSTEM:CONSOLE");
                    }
                    else
                    {
                        Arguments.Add("/SUBSYSTEM:WINDOWS");
                    }
                }
            }

            //
            //	Shipping & LTCG
            //
            if (LinkEnvironment.Configuration == CppConfiguration.Shipping)
            {
                // Use link-time code generation.
                Arguments.Add("/LTCG");
            }
        }

        public override CPPOutput CompileCPPFiles(CppCompileEnvironment CompileEnvironment, List<FileItem> InputFiles, DirectoryReference OutputDir, string ModuleName, ActionGraph ActionGraph)
        {
            List<string> SharedArguments = new List<string>();
            AppendCLArguments_Global(CompileEnvironment, SharedArguments);

            // Add include paths to the argument list.
            foreach (DirectoryReference IncludePath in CompileEnvironment.IncludePaths.UserIncludePaths)
            {
                AddIncludePath(SharedArguments, IncludePath);
            }

            foreach (DirectoryReference IncludePath in CompileEnvironment.IncludePaths.SystemIncludePaths)
            {
                AddSystemIncludePath(SharedArguments, IncludePath);
            }

            foreach (DirectoryReference IncludePath in EnvVars.IncludePaths)
            {
                AddSystemIncludePath(SharedArguments, IncludePath);
            }

            if (CompileEnvironment.bPrintTimingInfo)
            {
                SharedArguments.Add("/Bt+ /d2cgsummary");
                if (EnvVars.ToolChainVersion >= VersionNumber.Parse("14.14.26316"))
                {
                    SharedArguments.Add("/d1reportTime");
                }
            }

            // Add preprocessor definitions to the argument list.
            foreach (string Definition in CompileEnvironment.Definitions)
            {
                // Escape all quotation marks so that they get properly passed with the command line.
                string DefinitionArgument = Definition.Contains("\"") ? Definition.Replace("\"", "\\\"") : Definition;
                AddDefinition(SharedArguments, DefinitionArgument);
            }

            // Create a compile action for each source file.
            CPPOutput Result = new CPPOutput();
            foreach (FileItem SourceFile in InputFiles)
            {
                Action CompileAction = ActionGraph.Add(ActionType.Compile);
                CompileAction.CommandDescription = "Compile";
                CompileAction.bUseIncrementalLinking = CompileEnvironment.bUseIncrementalLinking;
                // ensure compiler timings are captured when we execute the action.
                if (CompileEnvironment.bPrintTimingInfo)
                {
                    CompileAction.bPrintDebugInfo = true;
                }

                List<string> FileArguments = new List<string>();
                bool bIsPlainCFile = Path.GetExtension(SourceFile.AbsolutePath).ToUpperInvariant() == ".C";

                // Add the C++ source file and its included files to the prerequisite item list.
                AddPrerequisiteSourceFile(CompileEnvironment, SourceFile, CompileAction.PrerequisiteItems);

                bool bEmitsObjectFile = true;
                if (CompileEnvironment.PrecompiledHeaderAction == PrecompiledHeaderAction.Create)
                {
                    // Generate a CPP File that just includes the precompiled header.
                    FileReference PCHCPPPath = CompileEnvironment.PrecompiledHeaderIncludeFilename.ChangeExtension(".cpp");
                    FileItem PCHCPPFile = FileItem.CreateIntermediateTextFile(
                        PCHCPPPath,
                        string.Format("#include \"{0}\"\r\n", CompileEnvironment.PrecompiledHeaderIncludeFilename.FullName.Replace('\\', '/'))
                        );

                    // Make sure the original source directory the PCH header file existed in is added as an include
                    // path -- it might be a private PCH header and we need to make sure that its found!
                    AddIncludePath(FileArguments, SourceFile.Location.Directory);

                    string PrecompiledFileExtension = UEBuildPlatform.GetBuildPlatform(UnrealTargetPlatform.Win64).GetBinaryExtension(UEBuildBinaryType.PrecompiledHeader);
                    // Add the precompiled header file to the produced items list.
                    FileItem PrecompiledHeaderFile = FileItem.GetItemByFileReference(
                        FileReference.Combine(
                            OutputDir,
                            Path.GetFileName(SourceFile.AbsolutePath) + PrecompiledFileExtension
                            )
                        );
                    CompileAction.ProducedItems.Add(PrecompiledHeaderFile);
                    Result.PrecompiledHeaderFile = PrecompiledHeaderFile;

                    // Add the parameters needed to compile the precompiled header file to the command-line.
                    FileArguments.Add(String.Format("/Yc\"{0}\"", CompileEnvironment.PrecompiledHeaderIncludeFilename));
                    FileArguments.Add(String.Format("/Fp\"{0}\"", PrecompiledHeaderFile.AbsolutePath));

                    // If we're creating a PCH that will be used to compile source files for a library, we need
                    // the compiled modules to retain a reference to PCH's module, so that debugging information
                    // will be included in the library.  This is also required to avoid linker warning "LNK4206"
                    // when linking an application that uses this library.
                    if (CompileEnvironment.bIsBuildingLibrary)
                    {
                        // NOTE: The symbol name we use here is arbitrary, and all that matters is that it is
                        // unique per PCH module used in our library
                        string FakeUniquePCHSymbolName = CompileEnvironment.PrecompiledHeaderIncludeFilename.GetFileNameWithoutExtension();
                        FileArguments.Add(String.Format("/Yl{0}", FakeUniquePCHSymbolName));
                    }

                    FileArguments.Add(String.Format("\"{0}\"", PCHCPPFile.AbsolutePath));

                    CompileAction.StatusDescription = PCHCPPPath.GetFileName();
                }
                else
                {
                    if (CompileEnvironment.PrecompiledHeaderAction == PrecompiledHeaderAction.Include)
                    {
                        CompileAction.bIsUsingPCH = true;
                        CompileAction.PrerequisiteItems.Add(CompileEnvironment.PrecompiledHeaderFile);


                        FileArguments.Add(String.Format("/FI\"{0}\"", CompileEnvironment.PrecompiledHeaderIncludeFilename.FullName));
                        FileArguments.Add(String.Format("/Yu\"{0}\"", CompileEnvironment.PrecompiledHeaderIncludeFilename.FullName));
                        FileArguments.Add(String.Format("/Fp\"{0}\"", CompileEnvironment.PrecompiledHeaderFile.AbsolutePath));
                    }

                    // Add the source file path to the command-line.
                    FileArguments.Add(String.Format("\"{0}\"", SourceFile.AbsolutePath));

                    CompileAction.StatusDescription = Path.GetFileName(SourceFile.AbsolutePath);
                }

                foreach (FileItem ForceIncludeFile in CompileEnvironment.ForceIncludeFiles)
                {
                    FileArguments.Add(String.Format("/FI\"{0}\"", ForceIncludeFile.Location));
                }

                if (bEmitsObjectFile)
                {
                    string ObjectFileExtension = UEBuildPlatform.GetBuildPlatform(UnrealTargetPlatform.Win64).GetBinaryExtension(UEBuildBinaryType.Object);
                    // Add the object file to the produced item list.
                    string ObjectLeafFilename = Path.GetFileName(SourceFile.AbsolutePath) + ObjectFileExtension;
                    FileItem ObjectFile = FileItem.GetItemByFileReference(FileReference.Combine(OutputDir, ObjectLeafFilename));
                    if (Target.WindowsPlatform.ObjSrcMapFile != null)
                    {
                        using (StreamWriter Writer = File.AppendText(Target.WindowsPlatform.ObjSrcMapFile))
                        {
                            Writer.WriteLine(string.Format("\"{0}\" -> \"{1}\"", ObjectLeafFilename, SourceFile.AbsolutePath));
                        }
                    }
                    CompileAction.ProducedItems.Add(ObjectFile);
                    Result.ObjectFiles.Add(ObjectFile);
                    FileArguments.Add(String.Format("/Fo\"{0}\"", ObjectFile.AbsolutePath));
                }

                // Create PDB files if we were configured to do that.
                if (CompileEnvironment.bUsePDBFiles)
                {
                    string PDBFileName;
                    bool bActionProducesPDB = false;

                    // All files using the same PCH are required to share the same PDB that was used when compiling the PCH
                    if (CompileEnvironment.PrecompiledHeaderAction == PrecompiledHeaderAction.Include)
                    {
                        PDBFileName = CompileEnvironment.PrecompiledHeaderIncludeFilename.GetFileName();
                    }
                    // Files creating a PCH use a PDB per file.
                    else if (CompileEnvironment.PrecompiledHeaderAction == PrecompiledHeaderAction.Create)
                    {
                        PDBFileName = CompileEnvironment.PrecompiledHeaderIncludeFilename.GetFileName();
                        bActionProducesPDB = true;
                    }
                    // Ungrouped C++ files use a PDB per file.
                    else if (!bIsPlainCFile)
                    {
                        PDBFileName = Path.GetFileName(SourceFile.AbsolutePath);
                        bActionProducesPDB = true;
                    }
                    // Group all plain C files that doesn't use PCH into the same PDB
                    else
                    {
                        PDBFileName = "MiscPlainC";
                    }

                    {
                        // Specify the PDB file that the compiler should write to.
                        FileItem PDBFile = FileItem.GetItemByFileReference(
                                FileReference.Combine(
                                    OutputDir,
                                    PDBFileName + ".pdb"
                                    )
                                );
                        FileArguments.Add(String.Format("/Fd\"{0}\"", PDBFile.AbsolutePath));

                        // Only use the PDB as an output file if we want PDBs and this particular action is
                        // the one that produces the PDB (as opposed to no debug info, where the above code
                        // is needed, but not the output PDB, or when multiple files share a single PDB, so
                        // only the action that generates it should count it as output directly)
                        if (bActionProducesPDB)
                        {
                            CompileAction.ProducedItems.Add(PDBFile);
                            Result.DebugDataFiles.Add(PDBFile);
                        }
                    }
                }

                // Add C or C++ specific compiler arguments.
                if (bIsPlainCFile)
                {
                    AppendCLArguments_C(FileArguments);
                }
                else
                {
                    AppendCLArguments_CPP(CompileEnvironment, FileArguments);
                }

                CompileAction.WorkingDirectory = UnrealBuildTool.EngineSourceDirectory.FullName;
                CompileAction.CommandPath = EnvVars.CompilerPath.FullName;
                CompileAction.PrerequisiteItems.AddRange(CompileEnvironment.ForceIncludeFiles);

                string[] AdditionalArguments = String.IsNullOrEmpty(CompileEnvironment.AdditionalArguments) ? new string[0] : new string[] { CompileEnvironment.AdditionalArguments };

                if (!ProjectFileGenerator.bGenerateProjectFiles && CompileAction.ProducedItems.Count > 0)
                {
                    FileItem TargetFile = CompileAction.ProducedItems[0];
                    FileReference ResponseFileName = new FileReference(TargetFile.AbsolutePath + ".response");
                    FileItem ResponseFileItem = FileItem.CreateIntermediateTextFile(ResponseFileName, SharedArguments.Concat(FileArguments).Concat(AdditionalArguments).Select(x => ActionThread.ExpandEnvironmentVariables(x)));
                    CompileAction.CommandArguments = " @\"" + ResponseFileName + "\"";
                    CompileAction.PrerequisiteItems.Add(ResponseFileItem);
                }
                else
                {
                    CompileAction.CommandArguments = String.Join(" ", SharedArguments.Concat(FileArguments).Concat(AdditionalArguments));
                }

                if (CompileEnvironment.PrecompiledHeaderAction == PrecompiledHeaderAction.Create)
                {
                    Log.TraceVerbose("Creating PCH " + CompileEnvironment.PrecompiledHeaderIncludeFilename + ": \"" + CompileAction.CommandPath + "\"" + CompileAction.CommandArguments);
                }
                else
                {
                    Log.TraceVerbose("   Compiling " + CompileAction.StatusDescription + ": \"" + CompileAction.CommandPath + "\"" + CompileAction.CommandArguments);
                }

                // VC++ always outputs the source file name being compiled, so we don't need to emit this ourselves
                CompileAction.bShouldOutputStatusDescription = false;

                // Don't farm out creation of precompiled headers as it is the critical path task.
                CompileAction.bCanExecuteRemotely =
                    CompileEnvironment.PrecompiledHeaderAction != PrecompiledHeaderAction.Create ||
                    CompileEnvironment.bAllowRemotelyCompiledPCHs
                    ;

                // When compiling with SN-DBS, modules that contain a #import must be built locally
                if (CompileEnvironment.bBuildLocallyWithSNDBS == true)
                {
                    CompileAction.bCanExecuteRemotelyWithSNDBS = false;
                }
            }
            return Result;
        }

        public override CPPOutput CompileRCFiles(CppCompileEnvironment CompileEnvironment, List<FileItem> InputFiles, DirectoryReference OutputDir, ActionGraph ActionGraph)
        {
            CPPOutput Result = new CPPOutput();

            foreach (FileItem RCFile in InputFiles)
            {
                Action CompileAction = ActionGraph.Add(ActionType.Compile);
                CompileAction.CommandDescription = "Resource";
                CompileAction.WorkingDirectory = UnrealBuildTool.EngineSourceDirectory.FullName;
                CompileAction.CommandPath = EnvVars.ResourceCompilerPath.FullName;
                CompileAction.StatusDescription = Path.GetFileName(RCFile.AbsolutePath);
                CompileAction.PrerequisiteItems.AddRange(CompileEnvironment.ForceIncludeFiles);

                // Resource tool can run remotely if possible
                CompileAction.bCanExecuteRemotely = true;

                List<string> Arguments = new List<string>();

                // Suppress header spew
                Arguments.Add("/nologo");

                // If we're compiling for 64-bit Windows, also add the _WIN64 definition to the resource
                // compiler so that we can switch on that in the .rc file using #ifdef.
                if (CompileEnvironment.Platform == CppPlatform.Win64)
                {
                    AddDefinition(Arguments, "_WIN64");
                }

                // Language
                Arguments.Add("/l 0x409");

                // Include paths. Don't use AddIncludePath() here, since it uses the full path and exceeds the max command line length.
                foreach (DirectoryReference IncludePath in CompileEnvironment.IncludePaths.UserIncludePaths)
                {
                    Arguments.Add(String.Format("/I \"{0}\"", IncludePath));
                }

                // System include paths.
                foreach (DirectoryReference SystemIncludePath in CompileEnvironment.IncludePaths.SystemIncludePaths)
                {
                    Arguments.Add(String.Format("/I \"{0}\"", SystemIncludePath));
                }
                foreach (DirectoryReference SystemIncludePath in EnvVars.IncludePaths)
                {
                    Arguments.Add(String.Format("/I \"{0}\"", SystemIncludePath));
                }

                // Preprocessor definitions.
                foreach (string Definition in CompileEnvironment.Definitions)
                {
                    if (!Definition.Contains("_API"))
                    {
                        AddDefinition(Arguments, Definition);
                    }
                }

                // Add the RES file to the produced item list.
                FileItem CompiledResourceFile = FileItem.GetItemByFileReference(
                    FileReference.Combine(
                        OutputDir,
                        Path.GetFileName(RCFile.AbsolutePath) + ".res"
                        )
                    );
                CompileAction.ProducedItems.Add(CompiledResourceFile);
                Arguments.Add(String.Format("/fo \"{0}\"", CompiledResourceFile.AbsolutePath));
                Result.ObjectFiles.Add(CompiledResourceFile);

                // Add the RC file as a prerequisite of the action.
                Arguments.Add(String.Format(" \"{0}\"", RCFile.AbsolutePath));

                CompileAction.CommandArguments = String.Join(" ", Arguments);

                // Add the C++ source file and its included files to the prerequisite item list.
                AddPrerequisiteSourceFile(CompileEnvironment, RCFile, CompileAction.PrerequisiteItems);
            }

            return Result;
        }

        public override FileItem LinkFiles(LinkEnvironment LinkEnvironment, bool bBuildImportLibraryOnly, ActionGraph ActionGraph)
        {
            if (LinkEnvironment.bIsBuildingDotNetAssembly)
            {
                return FileItem.GetItemByFileReference(LinkEnvironment.OutputFilePath);
            }

            bool bIsBuildingLibrary = LinkEnvironment.bIsBuildingLibrary || bBuildImportLibraryOnly;

            // Get link arguments.
            List<string> Arguments = new List<string>();
            if (bIsBuildingLibrary)
            {
                AppendLibArguments(LinkEnvironment, Arguments);
            }
            else
            {
                AppendLinkArguments(LinkEnvironment, Arguments);
            }

            if (LinkEnvironment.bPrintTimingInfo)
            {
                Arguments.Add("/time+");
            }

            // If we're only building an import library, add the '/DEF' option that tells the LIB utility
            // to simply create a .LIB file and .EXP file, and don't bother validating imports
            if (bBuildImportLibraryOnly)
            {
                Arguments.Add("/DEF");

                // Ensure that the import library references the correct filename for the linked binary.
                Arguments.Add(String.Format("/NAME:\"{0}\"", LinkEnvironment.OutputFilePath.GetFileName()));

                // Ignore warnings about object files with no public symbols.
                Arguments.Add("/IGNORE:4221");
            }


            if (!bIsBuildingLibrary)
            {
                // Delay-load these DLLs.
                foreach (string DelayLoadDLL in LinkEnvironment.DelayLoadDLLs.Distinct())
                {
                    Arguments.Add(String.Format("/DELAYLOAD:\"{0}\"", DelayLoadDLL));
                }

                // Pass the module definition file to the linker if we have one
                if (LinkEnvironment.ModuleDefinitionFile != null && LinkEnvironment.ModuleDefinitionFile.Length > 0)
                {
                    Arguments.Add(String.Format("/DEF:\"{0}\"", LinkEnvironment.ModuleDefinitionFile));
                }
            }

            // Set up the library paths for linking this binary
            if (bBuildImportLibraryOnly)
            {
                // When building an import library, ignore all the libraries included via embedded #pragma lib declarations. 
                // We shouldn't need them to generate exports.
                Arguments.Add("/NODEFAULTLIB");
            }
            else if (!LinkEnvironment.bIsBuildingLibrary)
            {
                // Add the library paths to the argument list.
                foreach (DirectoryReference LibraryPath in LinkEnvironment.LibraryPaths)
                {
                    Arguments.Add(String.Format("/LIBPATH:\"{0}\"", LibraryPath));
                }
                foreach (DirectoryReference LibraryPath in EnvVars.LibraryPaths)
                {
                    Arguments.Add(String.Format("/LIBPATH:\"{0}\"", LibraryPath));
                }

                // Add the excluded default libraries to the argument list.
                foreach (string ExcludedLibrary in LinkEnvironment.ExcludedLibraries)
                {
                    Arguments.Add(String.Format("/NODEFAULTLIB:\"{0}\"", ExcludedLibrary));
                }
            }

            // For targets that are cross-referenced, we don't want to write a LIB file during the link step as that
            // file will clobber the import library we went out of our way to generate during an earlier step.  This
            // file is not needed for our builds, but there is no way to prevent MSVC from generating it when
            // linking targets that have exports.  We don't want this to clobber our LIB file and invalidate the
            // existing timstamp, so instead we simply emit it with a different name
            FileReference ImportLibraryFilePath = FileReference.Combine(LinkEnvironment.IntermediateDirectory,
                                                         LinkEnvironment.OutputFilePath.GetFileNameWithoutExtension() + ".lib");

            if (LinkEnvironment.bIsCrossReferenced && !bBuildImportLibraryOnly)
            {
                ImportLibraryFilePath = ImportLibraryFilePath.ChangeExtension(".suppressed" + ImportLibraryFilePath.GetExtension());
            }

            FileItem OutputFile;
            if (bBuildImportLibraryOnly)
            {
                OutputFile = FileItem.GetItemByFileReference(ImportLibraryFilePath);
            }
            else
            {
                OutputFile = FileItem.GetItemByFileReference(LinkEnvironment.OutputFilePath);
                OutputFile.bNeedsHotReloadNumbersDLLCleanUp = LinkEnvironment.bIsBuildingDLL;
            }

            List<FileItem> ProducedItems = new List<FileItem>();
            ProducedItems.Add(OutputFile);

            List<FileItem> PrerequisiteItems = new List<FileItem>();

            // Add the input files to a response file, and pass the response file on the command-line.
            List<string> InputFileNames = new List<string>();
            foreach (FileItem InputFile in LinkEnvironment.InputFiles)
            {
                InputFileNames.Add(string.Format("\"{0}\"", InputFile.AbsolutePath));
                PrerequisiteItems.Add(InputFile);
            }

            if (!bBuildImportLibraryOnly)
            {
                // Add input libraries as prerequisites, too!
                foreach (FileItem InputLibrary in LinkEnvironment.InputLibraries)
                {
                    InputFileNames.Add(string.Format("\"{0}\"", InputLibrary.AbsolutePath));
                    PrerequisiteItems.Add(InputLibrary);
                }
            }

            if (!bIsBuildingLibrary)
            {
                foreach (string AdditionalLibrary in LinkEnvironment.AdditionalLibraries)
                {
                    InputFileNames.Add(string.Format("\"{0}\"", AdditionalLibrary));

                    // If the library file name has a relative path attached (rather than relying on additional
                    // lib directories), then we'll add it to our prerequisites list.  This will allow UBT to detect
                    // when the binary needs to be relinked because a dependent external library has changed.
                    //if( !String.IsNullOrEmpty( Path.GetDirectoryName( AdditionalLibrary ) ) )
                    {
                        PrerequisiteItems.Add(FileItem.GetItemByPath(AdditionalLibrary));
                    }
                }
            }

            Arguments.AddRange(InputFileNames);

            // Add the output file to the command-line.
            Arguments.Add(String.Format("/OUT:\"{0}\"", OutputFile.AbsolutePath));

            if (bBuildImportLibraryOnly || (LinkEnvironment.bHasExports && !bIsBuildingLibrary))
            {
                // An export file is written to the output directory implicitly; add it to the produced items list.
                FileReference ExportFilePath = ImportLibraryFilePath.ChangeExtension(".exp");
                FileItem ExportFile = FileItem.GetItemByFileReference(ExportFilePath);
                if(bBuildImportLibraryOnly) ProducedItems.Add(ExportFile);
            }

            if (!bIsBuildingLibrary)
            {
                // There is anything to export
                if (LinkEnvironment.bHasExports
                    // Shipping monolithic builds don't need exports
                    && (!((LinkEnvironment.Configuration == CppConfiguration.Shipping) && (LinkEnvironment.bShouldCompileMonolithic != false))))
                {
                    // Write the import library to the output directory for nFringe support.
                    FileItem ImportLibraryFile = FileItem.GetItemByFileReference(ImportLibraryFilePath);
                    Arguments.Add(String.Format("/IMPLIB:\"{0}\"", ImportLibraryFilePath));
                    if(bBuildImportLibraryOnly) ProducedItems.Add(ImportLibraryFile);
                }

                if (LinkEnvironment.bCreateDebugInfo)
                {
                    // Write the PDB file to the output directory.			
                    {
                        FileReference PDBFilePath = FileReference.Combine(LinkEnvironment.OutputDirectory, Path.GetFileNameWithoutExtension(OutputFile.AbsolutePath) + ".pdb");
                        FileItem PDBFile = FileItem.GetItemByFileReference(PDBFilePath);
                        Arguments.Add(String.Format("/PDB:\"{0}\"", PDBFilePath));
                        ProducedItems.Add(PDBFile);
                    }

                    // Write the MAP file to the output directory.			
                    if (LinkEnvironment.bCreateMapFile)
                    {
                        FileReference MAPFilePath = FileReference.Combine(LinkEnvironment.OutputDirectory, Path.GetFileNameWithoutExtension(OutputFile.AbsolutePath) + ".map");
                        FileItem MAPFile = FileItem.GetItemByFileReference(MAPFilePath);
                        Arguments.Add(String.Format("/MAP:\"{0}\"", MAPFilePath));
                        ProducedItems.Add(MAPFile);

                        // Export a list of object file paths, so we can locate the object files referenced by the map file
                        ExportObjectFilePaths(LinkEnvironment, Path.ChangeExtension(MAPFilePath.FullName, ".objpaths"));
                    }
                }

                // Add the additional arguments specified by the environment.
                if (!String.IsNullOrEmpty(LinkEnvironment.AdditionalArguments))
                {
                    Arguments.Add(LinkEnvironment.AdditionalArguments.Trim());
                }
            }

            // Create a response file for the linker, unless we're generating IntelliSense data
            FileReference ResponseFileName = GetResponseFileName(LinkEnvironment, OutputFile);
            if (!ProjectFileGenerator.bGenerateProjectFiles)
            {
                FileItem ResponseFile = FileItem.CreateIntermediateTextFile(ResponseFileName, String.Join(Environment.NewLine, Arguments));
                PrerequisiteItems.Add(ResponseFile);
            }

            // Create an action that invokes the linker.
            Action LinkAction = ActionGraph.Add(ActionType.Link);
            LinkAction.CommandDescription = "Link";
            LinkAction.WorkingDirectory = UnrealBuildTool.EngineSourceDirectory.FullName;
            LinkAction.CommandPath = bIsBuildingLibrary ? EnvVars.LibraryManagerPath.FullName : EnvVars.LinkerPath.FullName;
            LinkAction.CommandArguments = String.Format("@\"{0}\"", ResponseFileName);
            LinkAction.ProducedItems.AddRange(ProducedItems);
            LinkAction.PrerequisiteItems.AddRange(PrerequisiteItems);
            LinkAction.StatusDescription = Path.GetFileName(OutputFile.AbsolutePath);
            LinkAction.bUseIncrementalLinking = LinkEnvironment.bUseIncrementalLinking;

            // ensure compiler timings are captured when we execute the action.
            if (LinkEnvironment.bPrintTimingInfo)
            {
                LinkAction.bPrintDebugInfo = true;
            }

            // VS 15.3+ does not touch lib files if they do not contain any modifications, but we need to ensure the timestamps are updated to avoid repeatedly building them.
            if (bBuildImportLibraryOnly || (LinkEnvironment.bHasExports && !bIsBuildingLibrary))
            {
                LinkAction.bShouldDeleteProducedItems = true;
            }

            // Tell the action that we're building an import library here and it should conditionally be
            // ignored as a prerequisite for other actions
            LinkAction.bProducesImportLibrary = bBuildImportLibraryOnly || LinkEnvironment.bIsBuildingDLL;

            // Allow remote linking.  Especially in modular builds with many small DLL files, this is almost always very efficient
            LinkAction.bCanExecuteRemotely = true;

            Log.TraceVerbose("     Linking: " + LinkAction.StatusDescription);
            Log.TraceVerbose("     Command: " + LinkAction.CommandArguments);

            return OutputFile;
        }

        private void ExportObjectFilePaths(LinkEnvironment LinkEnvironment, string FileName)
        {
            // Write the list of object file directories
            HashSet<DirectoryReference> ObjectFileDirectories = new HashSet<DirectoryReference>();
            foreach (FileItem InputFile in LinkEnvironment.InputFiles)
            {
                ObjectFileDirectories.Add(InputFile.Location.Directory);
            }
            foreach (FileItem InputLibrary in LinkEnvironment.InputLibraries)
            {
                ObjectFileDirectories.Add(InputLibrary.Location.Directory);
            }
            foreach (string AdditionalLibrary in LinkEnvironment.AdditionalLibraries)
            {
                // Need to handle import libraries that are about to be built (but may not exist yet), third party libraries with relative paths in the UE4 tree, and system libraries in the system path
                FileReference AdditionalLibraryLocation = new FileReference(AdditionalLibrary);
                if (Path.IsPathRooted(AdditionalLibrary) || FileReference.Exists(AdditionalLibraryLocation))
                {
                    ObjectFileDirectories.Add(AdditionalLibraryLocation.Directory);
                }
            }
            foreach (DirectoryReference LibraryPath in LinkEnvironment.LibraryPaths)
            {
                ObjectFileDirectories.Add(LibraryPath);
            }
            foreach (string LibraryPath in (Environment.GetEnvironmentVariable("LIB") ?? "").Split(new char[] { ';' }, StringSplitOptions.RemoveEmptyEntries))
            {
                ObjectFileDirectories.Add(new DirectoryReference(LibraryPath));
            }
            Directory.CreateDirectory(Path.GetDirectoryName(FileName));
            File.WriteAllLines(FileName, ObjectFileDirectories.Select(x => x.FullName).OrderBy(x => x).ToArray());
        }

        /// <summary>
        /// Gets the default include paths for the given platform.
        /// </summary>
        public static string GetVCIncludePaths(CppPlatform Platform, WindowsCompiler Compiler, string CompilerVersion)
        {
            Debug.Assert(Platform == CppPlatform.Win32 || Platform == CppPlatform.Win64);

            // Make sure we've got the environment variables set up for this target
            VCEnvironment EnvVars = VCEnvironment.Create(Compiler, Platform, CompilerVersion, null);

            // Also add any include paths from the INCLUDE environment variable.  MSVC is not necessarily running with an environment that
            // matches what UBT extracted from the vcvars*.bat using SetEnvironmentVariablesFromBatchFile().  We'll use the variables we
            // extracted to populate the project file's list of include paths
            // @todo projectfiles: Should we only do this for VC++ platforms?
            StringBuilder IncludePaths = new StringBuilder();
            foreach (DirectoryReference IncludePath in EnvVars.IncludePaths)
            {
                IncludePaths.AppendFormat("{0};", IncludePath);
            }
            return IncludePaths.ToString();
        }

        public override void ModifyBuildProducts(ReadOnlyTargetRules Target, UEBuildBinary Binary, List<string> Libraries, List<UEBuildBundleResource> BundleResources, Dictionary<FileReference, BuildProductType> BuildProducts)
        {
            if (Binary.Type == UEBuildBinaryType.DynamicLinkLibrary)
            {
                BuildProducts.Add(FileReference.Combine(Binary.IntermediateDirectory, Binary.OutputFilePath.GetFileNameWithoutExtension() + ".lib"), BuildProductType.ImportLibrary);
            }
            if (Binary.Type == UEBuildBinaryType.Executable && Target.bCreateMapFile)
            {
                foreach (FileReference OutputFilePath in Binary.OutputFilePaths)
                {
                    BuildProducts.Add(FileReference.Combine(OutputFilePath.Directory, OutputFilePath.GetFileNameWithoutExtension() + ".map"), BuildProductType.MapFile);
                    BuildProducts.Add(FileReference.Combine(OutputFilePath.Directory, OutputFilePath.GetFileNameWithoutExtension() + ".objpaths"), BuildProductType.MapFile);
                }
            }
        }
    }
}
