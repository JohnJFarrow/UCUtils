
#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UCUtilitySubsystem.generated.h"


class FProperty;

/*
UCLASS()
class UUCUtilitySubsystem : public UEditorSubsystem
{
	GENERATED_BODY()
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;
};
*/

UCLASS()
class UCUTILS_API UUCUtilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	// get a string description of the type of a property
	// UFUNCTION(BlueprintCallable, Category = "Utilities")
	static FString GetTypeOfProperty(const FProperty* Property);
};