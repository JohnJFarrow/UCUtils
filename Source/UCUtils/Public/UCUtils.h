// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "Data/Registry/PCGGetDataFunctionRegistry.h" 

class FUCUtilsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void PreExit();

private:
	FPCGGetDataFunctionRegistry::FFunctionHandle GetComponentDataFunctionHandle;
};
