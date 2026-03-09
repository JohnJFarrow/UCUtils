// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Elements/PCGDataFromActor.h"

#include "PCGMaterialDataSettings.generated.h"

struct FPCGGetDataFunctionRegistryOutput;
struct FPCGGetDataFunctionRegistryParams;

UCLASS(MinimalAPI, BlueprintType, ClassGroup = (Procedural))
class UPCGGetMaterialDataSettings : public UPCGSettings
{
	GENERATED_BODY()

public:

	UPCGGetMaterialDataSettings();

	//~Begin UPCGSettings interface
	#if WITH_EDITOR
	virtual FName GetDefaultNodeName() const override;
	virtual FText GetDefaultNodeTitle() const override;
	#endif

protected:
	virtual TArray<FPCGPinProperties> InputPinProperties() const override;
	virtual TArray<FPCGPinProperties> OutputPinProperties() const override;
	virtual FPCGElementPtr CreateElement() const override;
	//~End UPCGSettings interface

};


class FPCGGetMaterialDataSettingsElement : public IPCGElement
{
protected:
	// Begin IPCGElement interface
	virtual bool ExecuteInternal(FPCGContext* Context) const override;
	// End IPCGElement interface
};


