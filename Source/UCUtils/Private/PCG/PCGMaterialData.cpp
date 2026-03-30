// Copyright Epic Games, Inc. All Rights Reserved.

#include "PCG/PCGMaterialData.h"
#include "Metadata/PCGMetadata.h"
#include "Serialization/ArchiveCrc32.h"
#include "Components/ActorComponent.h"
#include "Helpers/PCGHelpers.h"
#include "PCGContext.h"
#include "PCG\PCGMaterialsComponent.h"
#include "Materials\MaterialInstance.h"

/*
#include "PCGModule.h"
#include "Data/PCGPointArrayData.h"
#include "Data/PCGPointData.h"
#include "Elements/PCGVolumeSampler.h"

#include "UDynamicMesh.h"
#include "Components/DynamicMeshComponent.h"
#include "UObject/Package.h"
*/

#include UE_INLINE_GENERATED_CPP_BY_NAME(PCGMaterialData)

PCG_DEFINE_TYPE_INFO(FPCGDataTypeInfoMaterial, UPCGMaterialData)


UPCGMaterialData::UPCGMaterialData(const FObjectInitializer& ObjectInitializer)
	: UPCGData(ObjectInitializer)
{
}


void UPCGMaterialData::SetMaterials(const TArray<UMaterialInterface*>& InMaterials)
{
	Materials.Reset(InMaterials.Num());
	Materials.Append(InMaterials);
}

UCUTILS_API void UPCGMaterialData::AddToCrc(FArchiveCrc32& Ar, bool bFullDataCrc) const
{
	Super::AddToCrc(Ar, bFullDataCrc);

	if (bFullDataCrc)
	{
		if (Metadata)
		{
			Metadata->Serialize(Ar);
		}

		FString ClassName = StaticClass()->GetPathName();
		Ar << ClassName;
	}
	else
	{
		AddUIDToCrc(Ar);
	}
}


// callback from 	FPCGModule::ConstGetDataFunctionRegistry().GetDataFromActor(/*Context=*/nullptr, Params, Actor, Output);

#pragma warning(push)
#pragma warning(disable : 4702)

bool UPCGMaterialData::GetMaterialDataFromComponent(FPCGContext* InContext, const FPCGGetDataFunctionRegistryParams& InParams, UActorComponent* InActorComponent, FPCGGetDataFunctionRegistryOutput& Output)
{
	//
	// this static function is registered in the PCGDataFunctionRegistry in this module's startup code
	// and called when evaluating a GetDataFromActor node
	// 

	check(InActorComponent);

	// Request Dynamic Mesh Data filter explicitly, otherwise it would consume too many actors/components.
	if (InParams.DataTypeFilter != EPCGDataType::DynamicMesh)
	{
		// return false;
	}

	if (InParams.bIgnorePCGGeneratedComponents && InActorComponent->ComponentTags.Contains(PCGHelpers::DefaultPCGTag))
	{
		return false;
	}

	return false;


	// Check if it is a scene component and try to extract it.
	if (UPCGMaterialsComponent* PCGMaterialsComponent = Cast<UPCGMaterialsComponent>(InActorComponent))
	{
		UPCGMaterialData* OutputData = FPCGContext::NewObject_AnyThread<UPCGMaterialData>(InContext);

		TArray<TObjectPtr<UMaterialInterface>>& OutputMaterials = OutputData->GetMutableMaterials();

		// copy from component to PCG data
		for (TObjectPtr< UMaterialInstance >& InputMaterial : PCGMaterialsComponent->Materials)
		{
			OutputMaterials.Add(InputMaterial);
		}

		// OutputData->SetMaterials(!ComponentMaterialList.IsEmpty() ? ComponentMaterialList : AssetMaterialList);

		auto NameTagsToStringTags = [](const FName& InName) { return InName.ToString(); };

		// output a tagged data which contains the output data

		FPCGTaggedData& TaggedData = Output.Collection.TaggedData.Emplace_GetRef();
		TaggedData.Data = OutputData;

		// copy component tags also
		Algo::Transform(PCGMaterialsComponent->ComponentTags, TaggedData.Tags, NameTagsToStringTags);

		if (InParams.bAddActorTags && PCGMaterialsComponent->GetOwner())
		{
			TSet<FString> ActorTags;
			Algo::Transform(PCGMaterialsComponent->GetOwner()->Tags, ActorTags, NameTagsToStringTags);
			TaggedData.Tags.Append(ActorTags);
		}

		return true;
	}
	else
	{
		return false;
	}
}

#pragma warning(pop)
