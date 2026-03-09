// Copyright Epic Games, Inc. All Rights Reserved.

#include "UCUtils.h"
#include "PCG/PCGMaterialData.h"
#include "PCGModule.h"
#include "Misc/CoreDelegates.h"
//#include "Modules/ModuleManager.h"
//#include "Data/PCGDynamicMeshData.h"
//#include "Elements/PCGGetDynamicMeshData.h"

#define LOCTEXT_NAMESPACE "FUCUtilsModule"

void FUCUtilsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	// register a callback which is called from the GetDataFromActor node
	FModuleManager::Get().LoadModuleChecked(TEXT("PCG"));

	FPCGGetDataFunctionRegistry& PCGDataFunctionRegistry = FPCGModule::MutableGetDataFunctionRegistry();

	GetComponentDataFunctionHandle = PCGDataFunctionRegistry.RegisterDataFromComponentFunction(&UPCGMaterialData::GetMaterialDataFromComponent);

	// Register onto the PreExit, because we need the class to be still valid to remove them from the mapping
	FCoreDelegates::OnPreExit.AddRaw(this, &FUCUtilsModule::PreExit);
}


void FUCUtilsModule::PreExit()
{
	// No need to unregister if the PCG module is already dead.
	if (FPCGModule::IsPCGModuleLoaded())
	{
		FPCGGetDataFunctionRegistry& PCGDataFunctionRegistry = FPCGModule::MutableGetDataFunctionRegistry();
		PCGDataFunctionRegistry.UnregisterDataFromComponentFunction(GetComponentDataFunctionHandle);
	}
}


void FUCUtilsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUCUtilsModule, UCUtils)