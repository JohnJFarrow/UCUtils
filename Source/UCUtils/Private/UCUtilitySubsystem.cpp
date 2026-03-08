

#include "UCUtilitySubsystem.h"
#include "UObject/TextProperty.h"

/*
void UUCUtilitySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UUCUtilitySubsystem::Deinitialize()
{
	Super::Deinitialize();
}
*/

FString UUCUtilityLibrary::GetTypeOfProperty(const FProperty* Property)
{
	if (CastField< FStrProperty >(Property))
	{
		return "FString";
	}
	else if (CastField< FNameProperty >(Property))
	{
		return "FName";
	}
	else if (CastField< FBoolProperty >(Property))
	{
		return "bool";
	}
	else if (CastField< FIntProperty >(Property))
	{
		return "int";
	}
	else if (CastField< FInt16Property >(Property))
	{
		return "int16";
	}
	else if (CastField< FInt64Property >(Property))
	{
		return "int64";
	}
	else if (CastField< FUInt16Property >(Property))
	{
		return "FUInt16Property";
	}
	else if (CastField< FUInt32Property >(Property))
	{
		return "FUInt32Property";
	}
	else if (CastField< FUInt64Property >(Property))
	{
		return "FUInt64Property";
	}
	else if (CastField< FFloatProperty >(Property))
	{
		return "float";
	}
	else if (CastField< FDoubleProperty >(Property))
	{
		return "double";
	}
	else if (CastField< FByteProperty >(Property))
	{
		return "byte";
	}
	else if (const FClassProperty* ClassProperty = CastField< FClassProperty >(Property))
	{
		if (ClassProperty->PropertyClass)
		{
			return FString::Printf(TEXT("class %s"), *ClassProperty->PropertyClass->GetName());
		}
		else
		{
			return "class";
		}
	}
	else if (const FObjectProperty* ObjectProperty = CastField< FObjectProperty >(Property))
	{
		if (ObjectProperty->PropertyClass)
		{
			return FString::Printf(TEXT("object %s"), *ObjectProperty->PropertyClass->GetName());
		}
		else
		{
			return "object";
		}
	}
	else if (const FStructProperty* StructProp = CastField< FStructProperty >(Property))
	{
		// GetStructCPPName()
		return FString::Printf(TEXT("struct %s"), *StructProp->Struct->GetName());
	}
	else if (const FArrayProperty* ArrayProperty = CastField< FArrayProperty >(Property))
	{
		if (ArrayProperty->Inner)
		{
			FString SubType = GetTypeOfProperty(ArrayProperty->Inner);
			return FString("array") + "[" + SubType + "]";
		}
		else
		{
			return "array";
		}
	}
	else if (const FMapProperty* MapProperty = CastField< FMapProperty >(Property))
	{
		FString Result = { "Map[" };
		Result.Append(GetTypeOfProperty(MapProperty->GetKeyProperty()));
		Result.Append(",");
		Result.Append(GetTypeOfProperty(MapProperty->GetValueProperty()));
		Result.Append("]");
		return Result;
	}
	else if (const FSetProperty* SetProperty = CastField< FSetProperty >(Property))
	{
		FString Result = { "Set[" };
		Result.Append(GetTypeOfProperty(SetProperty->GetElementProperty()));
		Result.Append("]");
		return Result;
	}
	else if (const FEnumProperty* EnumProperty = CastField< FEnumProperty >(Property))
	{
		FString Result = { "Enum[" };
		const FName& Name = EnumProperty->GetEnum()->GetFName();
		Result.Append(*Name.ToString());
		Result.Append("]");
		return Result;
	}
	else if (const FSoftObjectProperty* SoftObjectProperty = CastField< FSoftObjectProperty >(Property))
	{
		const UClass* ReferencedClass = SoftObjectProperty->PropertyClass;
		if (ReferencedClass)
		{
			FString Name;
			ReferencedClass->GetName(Name);
			return TEXT("SoftObjectProperty[") + Name + TEXT("]");
		}
		return "softobjectproperty";
	}
	else if (CastField< FTextProperty >(Property))
	{
		return "text";
	}
	else if (const FWeakObjectProperty* WeakObjectProperty = CastField< FWeakObjectProperty >(Property))
	{
		const UClass* ReferencedClass = WeakObjectProperty->PropertyClass;
		if (ReferencedClass)
		{
			FString Name;
			ReferencedClass->GetName(Name);
			return TEXT("WeakObjectProperty[") + Name + TEXT("]");
		}
		return "weakobjectproperty";
	}
	else
	{
		return "unknown";
	}
}
