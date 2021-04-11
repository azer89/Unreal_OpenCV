// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

using System.IO;

using System;

public class unreal_ar : ModuleRules
{
	public unreal_ar(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"RHI", 
			"RenderCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

		LoadOpenCV();
	}

	// Read this webpage:
	// https://docs.unrealengine.com/en-US/ProductionPipelines/UnrealBuildSystem/ModuleFiles/index.html
	public void LoadOpenCV()
	{
		Console.WriteLine("===Loading OpenCV====");

		// Create OpenCV Path 
		string OpenCV_H_Path   = "C:/Users/azer/workspace/opencv_4_5_2/bin/install/include";
		
		PublicIncludePaths.Add(OpenCV_H_Path);

		// PublicLibraryPaths is a list of system/library paths containing .lib files
		// string OpenCV_LIB_Path = "C:/Users/azer/workspace/opencv_4_5_2/bin/install/x64/vc16/lib";
		// PublicLibraryPaths.Add(OpenCV_LIB_Path);	// this is obsolete :(
		// Add Static Libraries
		PublicAdditionalLibraries.Add("C:/Users/azer/workspace/opencv_4_5_2/bin/install/x64/vc16/lib/opencv_core452.lib");
		PublicAdditionalLibraries.Add("C:/Users/azer/workspace/opencv_4_5_2/bin/install/x64/vc16/lib/opencv_highgui452.lib");
		PublicAdditionalLibraries.Add("C:/Users/azer/workspace/opencv_4_5_2/bin/install/x64/vc16/lib/opencv_aruco452.lib");
		PublicAdditionalLibraries.Add("C:/Users/azer/workspace/opencv_4_5_2/bin/install/x64/vc16/lib/opencv_imgcodecs452.lib");
		PublicAdditionalLibraries.Add("C:/Users/azer/workspace/opencv_4_5_2/bin/install/x64/vc16/lib/opencv_calib3d452.lib");
		PublicAdditionalLibraries.Add("C:/Users/azer/workspace/opencv_4_5_2/bin/install/x64/vc16/lib/opencv_imgproc452.lib");
		PublicAdditionalLibraries.Add("C:/Users/azer/workspace/opencv_4_5_2/bin/install/x64/vc16/lib/opencv_videoio452.lib");
		PublicAdditionalLibraries.Add("C:/Users/azer/workspace/opencv_4_5_2/bin/install/x64/vc16/lib/opencv_features2d452.lib");
		PublicAdditionalLibraries.Add("C:/Users/azer/workspace/opencv_4_5_2/bin/install/x64/vc16/lib/opencv_flann452.lib");

		// Add Dynamic Libraries
		// Need to copy the DLL files to <project name>\Binaries\Win64
		// TODO: Need to read this:
		// https://docs.unrealengine.com/en-US/ProductionPipelines/BuildTools/UnrealBuildTool/ThirdPartyLibraries/index.html
		PublicDelayLoadDLLs.Add("opencv_core452.dll");
		PublicDelayLoadDLLs.Add("opencv_highgui452.dll");
		PublicDelayLoadDLLs.Add("opencv_aruco452.dll");
		PublicDelayLoadDLLs.Add("opencv_imgcodecs452.dll");
		PublicDelayLoadDLLs.Add("opencv_calib3d452.dll");
		PublicDelayLoadDLLs.Add("opencv_imgproc452.dll");
		PublicDelayLoadDLLs.Add("opencv_videoio452.dll");
		PublicDelayLoadDLLs.Add("opencv_features2d452.dll");
		PublicDelayLoadDLLs.Add("opencv_flann452.dll");
	}
}
