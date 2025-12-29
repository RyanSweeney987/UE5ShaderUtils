// Copyright Ryan Sweeney 2024, All Rights Reserved

#include "UE5ShaderUtils.h"

#include "Misc/Paths.h"
#include "ShaderCore.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FUE5ShaderUtilsModule"

void FUE5ShaderUtilsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	const FString PluginShaderDir = FPaths::Combine(IPluginManager::Get().FindPlugin(TEXT("UE5ShaderUtils"))->GetBaseDir(), TEXT("Shaders"));
	if(!AllShaderSourceDirectoryMappings().Contains(TEXT("/ShadersVirtualLocation")))
	{
		AddShaderSourceDirectoryMapping(TEXT("/ShadersVirtualLocation"), PluginShaderDir);
	}
}

void FUE5ShaderUtilsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUE5ShaderUtilsModule, UE5ShaderUtils)