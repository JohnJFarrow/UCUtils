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
	: UPCGSpatialData(ObjectInitializer)
{
}

/*
void UPCGDynamicMeshData::Initialize(UDynamicMesh* InMesh, bool bCanTakeOwnership, const TArray<UMaterialInterface*>& InMaterials)
{
	check(DynamicMesh);

	if (!InMesh)
	{
		UE_LOG(LogPCG, Error, TEXT("Dynamic Mesh initialization failed because InMesh is null."));
		return;
	}

	if (bCanTakeOwnership)
	{
		Initialize(std::move(InMesh->GetMeshRef()), InMaterials);
	}
	else
	{
		// Make a copy
		Initialize(UE::Geometry::FDynamicMesh3(InMesh->GetMeshRef()), InMaterials);
	}
}

void UPCGDynamicMeshData::Initialize(UE::Geometry::FDynamicMesh3&& InMesh, const TArray<UMaterialInterface*>& InMaterials)
{
	check(DynamicMesh);
	DynamicMesh->SetMesh(std::move(InMesh));
	ResetBounds();

	bDynamicMeshOctreeIsDirty = true;

	SetMaterials(InMaterials);
}

void UPCGDynamicMeshData::ResetBounds() const
{
	check(DynamicMesh);
	CachedBounds = static_cast<FBox>(DynamicMesh->GetMeshRef().GetBounds());
	bDynamicMeshBoundsAreDirty = false;
}

FBox UPCGDynamicMeshData::GetBounds() const
{
	if (bDynamicMeshBoundsAreDirty)
	{
		DynamicMeshBoundsLock.Lock();
		if (bDynamicMeshBoundsAreDirty)
		{
			ResetBounds();
		}
		DynamicMeshBoundsLock.Unlock();
	}

	return CachedBounds;
}

void UPCGDynamicMeshData::AddToCrc(FArchiveCrc32& Ar, bool bFullDataCrc) const
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

		Ar << DynamicMesh->GetMeshRef();
	}
	else
	{
		AddUIDToCrc(Ar);
	}
}

bool UPCGDynamicMeshData::SamplePoint(const FTransform& InTransform, const FBox& InBounds, FPCGPoint& OutPoint, UPCGMetadata* OutMetadata) const
{
	if (!DynamicMesh)
	{
		return false;
	}

	const FVector BoxCenter = InTransform.TransformPosition(InBounds.GetCenter());
	const FVector HalfScaledExtents(InBounds.GetExtent() * InTransform.GetScale3D());
	const UE::Geometry::FAxisAlignedBox3d ScaledBounds(BoxCenter - HalfScaledExtents, BoxCenter + HalfScaledExtents);

	if (!ScaledBounds.Intersects(GetBounds()))
	{
		return false;
	}

	const UE::Geometry::FDynamicMeshOctree3& Octree = GetDynamicMeshOctree();
	const UE::Geometry::FDynamicMesh3& MeshRef = DynamicMesh->GetMeshRef();


	// TODO: Awful way to do this, need to implement a triangle AABB box intersection if we want more precision. But not even sure that is the right approach using the Octree
	auto TriangleIntersectionTest = [this, &ScaledBounds, MeshRef](int32 TriangleID) -> bool
		{
			return ScaledBounds.Intersects(MeshRef.GetTriBounds(TriangleID));
		};

	const bool bIntersect = Octree.ParallelOverlapAnyQuery(ScaledBounds, TriangleIntersectionTest) != INDEX_NONE;

	if (bIntersect)
	{
		OutPoint.Transform = InTransform;
		OutPoint.SetLocalBounds(InBounds);
		OutPoint.Density = 1.0f;
		return true;
	}
	else
	{
		return false;
	}
}

const UE::Geometry::FDynamicMeshOctree3& UPCGDynamicMeshData::GetDynamicMeshOctree() const
{
	if (DynamicMesh && bDynamicMeshOctreeIsDirty)
	{
		FScopeLock Lock(&DynamicMeshOctreeLock);

		if (bDynamicMeshOctreeIsDirty)
		{
			if (DynamicMeshOctree.Mesh != DynamicMesh->GetMeshPtr())
			{
				// Was never initialized, or with another mesh, reset it
				if (DynamicMeshOctree.Mesh)
				{
					DynamicMeshOctree = UE::Geometry::FDynamicMeshOctree3{};
				}

				DynamicMeshOctree.Initialize(&DynamicMesh->GetMeshRef());
			}
			else
			{
				// We can re-insert all triangles to avoid recomputing the whole octree
				DynamicMeshOctree.ReinsertTriangles(DynamicMesh->GetMeshRef().TriangleIndicesItr());
			}

			bDynamicMeshOctreeIsDirty = false;
		}
	}

	return DynamicMeshOctree;
}

const UPCGBasePointData* UPCGDynamicMeshData::ToBasePointData(FPCGContext* Context, const FBox& InBounds, TSubclassOf<UPCGBasePointData> PointDataClass) const
{
	TRACE_CPUPROFILER_EVENT_SCOPE(UPCGDynamicMeshData::CreatePointData);

	check(DynamicMesh);

	TArray<FVector> Vertices;

	DynamicMesh->ProcessMesh([&Vertices](const FDynamicMesh3& ReadMesh)
		{
			Vertices.Reserve(ReadMesh.VertexCount());
			for (int32 vid : ReadMesh.VertexIndicesItr())
			{
				Vertices.Add(ReadMesh.GetVertex(vid));
			}
		});

	UPCGBasePointData* Data = FPCGContext::NewObject_AnyThread<UPCGBasePointData>(Context, GetTransientPackage(), PointDataClass);
	Data->SetNumPoints(Vertices.Num());
	Data->AllocateProperties(EPCGPointNativeProperties::Transform | EPCGPointNativeProperties::Seed);

	int32 NumWritten = 0;
	FPCGPointValueRanges OutRanges(Data, false);

	for (const FVector& Vertex : Vertices)
	{
		if (InBounds.IsValid && !InBounds.IsInside(Vertex))
		{
			continue;
		}

		OutRanges.TransformRange[NumWritten].SetLocation(Vertex);
		OutRanges.SeedRange[NumWritten] = PCGHelpers::ComputeSeedFromPosition(Vertex);

		++NumWritten;
	}

	Data->SetNumPoints(NumWritten);

	UE_LOG(LogPCG, Verbose, TEXT("Dynamic Mesh extracted %d points"), NumWritten);

	return Data;
}

const UPCGPointArrayData* UPCGDynamicMeshData::ToPointArrayData(FPCGContext* Context, const FBox& InBounds) const
{
	return CastChecked<UPCGPointArrayData>(ToBasePointData(Context, InBounds, UPCGPointArrayData::StaticClass()));
}

const UPCGPointData* UPCGDynamicMeshData::ToPointData(FPCGContext* Context, const FBox& InBounds) const
{
	return CastChecked<UPCGPointData>(ToBasePointData(Context, InBounds, UPCGPointData::StaticClass()));
}


void UPCGDynamicMeshData::InitializeDynamicMeshComponentFromData(UDynamicMeshComponent* InComponent) const
{
	check(InComponent);

	InComponent->SetMesh(UE::Geometry::FDynamicMesh3(DynamicMesh->GetMeshRef()));
	for (int32 i = 0; i < Materials.Num(); ++i)
	{
		InComponent->SetMaterial(i, Materials[i]);
	}
}

UPCGSpatialData* UPCGDynamicMeshData::CopyInternal(FPCGContext* Context) const
{
	UPCGDynamicMeshData* NewDynamicMeshData = FPCGContext::NewObject_AnyThread<UPCGDynamicMeshData>(Context);

	NewDynamicMeshData->Initialize(DynamicMesh, false);
	NewDynamicMeshData->Materials = Materials;

	return NewDynamicMeshData;
}
*/

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
