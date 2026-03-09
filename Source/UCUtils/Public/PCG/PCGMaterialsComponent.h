// Copyright 2026 Timothé Lapetite and contributors
// Released under the MIT license https://opensource.org/license/MIT/


#pragma once

#include "CoreMinimal.h"
#include "Components/PrimitiveComponent.h"
#include "PCGMaterialsComponent.generated.h"

class UMaterialInstance;

UCLASS(Hidden, meta = (BlueprintSpawnableComponent), ClassGroup = Rendering)
class UCUTILS_API UPCGMaterialsComponent : public UPrimitiveComponent
{
	GENERATED_BODY()

	//
	// holds a list of materials and interacts with the GetActorData functions to work with the GetActorData node
	//

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray< TObjectPtr< UMaterialInstance >> Materials;
};

