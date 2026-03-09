// Copyright Epic Games, Inc. All Rights Reserved.

#include "UCUtils.h"

#define LOCTEXT_NAMESPACE "FUCUtilsModule"

void FUCUtilsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// register a callback which is called from the GetDataFromActor node
	FModuleManager::Get().LoadModuleChecked(TEXT("PCG"));
}

void FUCUtilsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUCUtilsModule, UCUtils)