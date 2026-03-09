// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Elements/PCGDataFromActor.h"

#include "PCGMaterialDataSettings.generated.h"

struct FPCGGetDataFunctionRegistryOutput;
struct FPCGGetDataFunctionRegistryParams;

UCLASS(MinimalAPI, BlueprintType, ClassGroup = (Procedural))
class UPCGGetMaterialDataSettings : public UPCGDataFromActorSettings
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
	virtual TArray<FPCGPinProperties> OutputPinProperties() const override;
	//~End UPCGSettings interface

	//~Begin UPCGDataFromActorSettings interface
public:
	virtual EPCGDataType GetDataFilter() const override
	{
		return EPCGDataType::Other;
	}

protected:
	#if WITH_EDITOR
	virtual bool DisplayModeSettings() const override;
	#endif
	//~End UPCGDataFromActorSettings
};

