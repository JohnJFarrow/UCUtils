
#pragma once

#include "PCGData.h"
#include "Data\PCGSpatialData.h"
#include "PCGMaterialData.generated.h"

struct FPCGContext;
struct FPCGGetDataFunctionRegistryParams;
struct FPCGGetDataFunctionRegistryOutput;
class UMaterialInterface;
class UActorComponent;


USTRUCT()
struct FPCGDataTypeInfoMaterial : public FPCGDataTypeInfo
{
	GENERATED_BODY()

	PCG_DECLARE_TYPE_INFO(UCUTILS_API);
};

UCLASS(MinimalAPI, BlueprintType, ClassGroup = (Procedural))
class UPCGMaterialData : public UPCGSpatialData
{
	GENERATED_BODY()

public:
	UCUTILS_API UPCGMaterialData(const FObjectInitializer& ObjectInitializer);

	//UCUTILS_API void Initialize(UDynamicMesh* InMesh, bool bCanTakeOwnership = false, const TArray<UMaterialInterface*>& InOptionalMaterials = {});
	//UCUTILS_API void Initialize(UE::Geometry::FDynamicMesh3&& InMesh, const TArray<UMaterialInterface*>& InOptionalMaterials = {});

	// ~Begin UPCGData interface
	PCG_ASSIGN_DEFAULT_TYPE_INFO(FPCGDataTypeInfoMaterial);

	UFUNCTION(BlueprintCallable, Category = "DynamicMesh")
	UCUTILS_API void SetMaterials(const TArray<UMaterialInterface*>& InMaterials);

	UCUTILS_API virtual void AddToCrc(FArchiveCrc32& Ar, bool bFullDataCrc) const override;
	// ~End UPCGData interface

	TArray<TObjectPtr<UMaterialInterface>>& GetMutableMaterials()
	{
		return Materials;
	}

	const TArray<TObjectPtr<UMaterialInterface>>& GetMaterials() const
	{
		return Materials;
	}

public:
	static bool GetMaterialDataFromComponent(FPCGContext* InContext, const FPCGGetDataFunctionRegistryParams& InParams, UActorComponent* InActorComponent, FPCGGetDataFunctionRegistryOutput& Output);


	//UCUTILS_API virtual const UPCGPointData* ToPointData(FPCGContext* Context, const FBox& InBounds) const override;
	//UCUTILS_API virtual const UPCGPointArrayData* ToPointArrayData(FPCGContext* Context, const FBox& InBounds) const override;
	//~End UPCGSpatialData interface

private:
	// const but will set the mutable CachedBounds
	// void ResetBounds() const;
	//const UPCGBasePointData* ToBasePointData(FPCGContext* Context, const FBox& InBounds, TSubclassOf<UPCGBasePointData> PointDataClass) const;

protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Instanced, Category = "Materials")
	TArray<TObjectPtr<UMaterialInterface>> Materials;

};

