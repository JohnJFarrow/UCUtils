// Copyright Epic Games, Inc. All Rights Reserved.

#include "PCG/PCGMaterialDataSettings.h"

#if 0
#include "PCGContext.h"
#include "Data/PCGDynamicMeshData.h"
#include "Data/Registry/PCGGetDataFunctionRegistry.h"
#include "Helpers/PCGGeometryHelpers.h"
#include "Helpers/PCGHelpers.h"

#include "DynamicMeshActor.h"
#include "ConversionUtils/SceneComponentToDynamicMesh.h"
#include "Materials/MaterialInterface.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(PCGMaterialDataSettings)

#define LOCTEXT_NAMESPACE "PCGMaterialDataSettings"


namespace
{
	const FName NodeName("GetMaterialDataSettings");
	const FText NodeTitle = FText::Format(LOCTEXT("NodeTitle", "PCG Unrealcode | {0}"), FText::FromName(NodeName));
	const FText Tooltip = LOCTEXT("Tooltip", "Get specified property as array of material instance references");
}

UPCGGetMaterialDataSettings::UPCGGetMaterialDataSettings()
{
}

#if WITH_EDITOR
FName UPCGGetMaterialDataSettings::GetDefaultNodeName() const
{
	return NodeName;
}

FText UPCGGetMaterialDataSettings::GetDefaultNodeTitle() const
{
	return NodeTitle;
}
#endif 

TArray<FPCGPinProperties> UPCGGetMaterialDataSettings::InputPinProperties() const
{
	TArray<FPCGPinProperties> PinProperties;

	FPCGPinProperties& Pin = PinProperties.Emplace_GetRef(PCGPinConstants::DefaultInputLabel, FPCGDataTypeInfo::AsId());

	Pin.Usage = EPCGPinUsage::Normal;
	Pin.AllowedTypes = EPCGDataType::Other;
	Pin.bAllowMultipleData = true;

	return PinProperties;
}

TArray<FPCGPinProperties> UPCGGetMaterialDataSettings::OutputPinProperties() const
{
	TArray<FPCGPinProperties> PinProperties;
	PinProperties.Emplace(PCGPinConstants::DefaultOutputLabel, EPCGDataType::Other);

	return PinProperties;
}

FPCGElementPtr UPCGGetMaterialDataSettings::CreateElement() const
{
	return MakeShared< FPCGGetMaterialDataSettingsElement >();
}


bool FPCGGetMaterialDataSettingsElement::ExecuteInternal(FPCGContext* Context) const
{
	// make a material data
	// copy the input array into it
	// wrap it in a tagged data
	// output it

	return true;
}


#undef LOCTEXT_NAMESPACE
