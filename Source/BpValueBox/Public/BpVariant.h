#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UObject/NoExportTypes.h"
#include <UObject/UnrealType.h>
#include "UObject/UnrealTypePrivate.h"
#include <Kismet/KismetSystemLibrary.h>
#include "StructUtils/InstancedStruct.h"
#include "Misc/Optional.h"
#include "ValueType.h"
#include "BpVariant.generated.h"

/*
This struct will simply hold a TVariant with all the base Blueprint types, nothing more.
This will allow values to get passed around easily with value semantics instead of reference semantics.
As of now, this holds 56 bytes in memory.
*/
USTRUCT(BlueprintType)
struct FBpVariant
{
	GENERATED_BODY()

	TVariant<UObject*, UClass*, TSoftObjectPtr<UObject>, TSoftClassPtr<UObject>, FVariant, FText, FInstancedStruct>
	Data;
};

UCLASS()
class BPVALUEBOX_API UBpVariantStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static const bool Equals(const FBpVariant& Left, const FBpVariant& Right)
	{
		// If they don't have the same type, it's false
		if (Left.Data.GetIndex() != Right.Data.GetIndex())
		{
			return false;
		}
		if (Left.Data.IsType<FVariant>() && Right.Data.IsType<FVariant>())
		{
			return Left.Data.Get<FVariant>() == Right.Data.Get<FVariant>();
		}
		if (Left.Data.IsType<FText>() && Right.Data.IsType<FText>())
		{
			return Left.Data.Get<FText>().EqualTo(Right.Data.Get<FText>());
		}
		if (Left.Data.IsType<FInstancedStruct>() && Right.Data.IsType<FInstancedStruct>())
		{
			return Left.Data.Get<FInstancedStruct>() == Right.Data.Get<FInstancedStruct>();
		}
		if (Left.Data.IsType<UObject*>() && Right.Data.IsType<UObject*>())
		{
			return Left.Data.Get<UObject*>() == Right.Data.Get<UObject*>();
		}
		return false;
	}

	template <typename Type>
	static FBpVariant SetValue(FBpVariant& Variant, Type Value)
	{
		Variant.Data.Set<Type>(Value);
		return Variant;
	}

	static FBpVariant SetVariant(FBpVariant& Variant, FVariant Value)
	{
		return SetValue(Variant, Value);
	}

	static FBpVariant MakeFromFVariant(const FVariant& Value)
	{
		FBpVariant variant;
		return SetValue(variant, Value);
	}

	template <typename T>
	static FBpVariant MakeFromGeneric(const T& Value)
	{
		FBpVariant variant;
		return SetValue(variant, Value);
	}

	template <typename Type>
	static Type GetValue(FBpVariant Variant)
	{
		if (Type* value = Variant.Data.TryGet<Type>())
		{
			return *value;
		}
		return Type();
	}

	// Add error checking on this using GetType
	template <typename Type>
	static Type GetVariant(FBpVariant Variant)
	{
		FVariant variant = GetValue<FVariant>(Variant);
		return variant.GetValue<Type>();
	}

	UFUNCTION(BlueprintCallable, Category="BpVariant")
	static EValueType GetType(FBpVariant Variant)
	{
		if (Variant.Data.IsType<FVariant>())
		{
			FVariant value = Variant.Data.Get<FVariant>();
			switch (value.GetType())
			{
			case EVariantTypes::Bool:
				return EValueType::Bool;
			case EVariantTypes::UInt8:
				return EValueType::Byte;
			case EVariantTypes::Int32:
				return EValueType::Int32;
			case EVariantTypes::Int64:
				return EValueType::Int64;
			case EVariantTypes::Float:
				return EValueType::Float32;
			case EVariantTypes::Double:
				return EValueType::Float64;
			case EVariantTypes::Name:
				return EValueType::Name;
			case EVariantTypes::String:
				return EValueType::String;
			case EVariantTypes::Vector:
				return EValueType::Vector;
			case EVariantTypes::Rotator:
				return EValueType::Rotator;
			case EVariantTypes::Transform:
				return EValueType::Transform;
			default: ;
			}
		}
		if (Variant.Data.IsType<FText>())
		{
			return EValueType::Text;
		}
		if (Variant.Data.IsType<FInstancedStruct>())
		{
			return EValueType::Struct;
		}
		if (UObject** value = Variant.Data.TryGet<UObject*>())
		{
			if (value != nullptr)
			{
				return EValueType::Object;
			}
		}
		return EValueType::None;
	}

	UFUNCTION(BlueprintCallable, Category="BpVariant")
	static FBpVariant SetBool(UPARAM(ref)
	                          FBpVariant& Variant, const bool Value)
	{
		return SetVariant(Variant, Value);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static bool GetBool(FBpVariant Variant)
	{
		return GetVariant<bool>(Variant);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static FBpVariant MakeVariantFromBool(const bool Value)
	{
		FBpVariant variant;
		return SetVariant(variant, Value);
	}

	UFUNCTION(BlueprintCallable, Category="BpVariant")
	static FBpVariant SetByte(UPARAM(ref)
	                          FBpVariant& Variant, const uint8 Value)
	{
		return SetVariant(Variant, Value);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static uint8 GetByte(FBpVariant Variant)
	{
		return GetVariant<uint8>(Variant);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static FBpVariant MakeVariantFromByte(const uint8 Value)
	{
		FBpVariant variant;
		return SetVariant(variant, Value);
	}

	UFUNCTION(BlueprintCallable, Category="BpVariant")
	static FBpVariant SetInt(UPARAM(ref)
	                         FBpVariant& Variant, const int32 Value)
	{
		return SetVariant(Variant, Value);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static int32 GetInt(FBpVariant Variant)
	{
		return GetVariant<int32>(Variant);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static FBpVariant MakeVariantFromInt(const int32 Value)
	{
		FBpVariant variant;
		return SetVariant(variant, Value);
	}

	UFUNCTION(BlueprintCallable, Category="BpVariant")
	static FBpVariant SetInt64(UPARAM(ref)
	                           FBpVariant& Variant, const int64 Value)
	{
		return SetVariant(Variant, Value);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static int64 GetInt64(FBpVariant Variant)
	{
		return GetVariant<int64>(Variant);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static FBpVariant MakeVariantFromInt64(const int64 Value)
	{
		FBpVariant variant;
		return SetVariant(variant, Value);
	}

	UFUNCTION(BlueprintCallable, Category="BpVariant")
	static FBpVariant SetFloat(UPARAM(ref)
	                           FBpVariant& Variant, const float Value)
	{
		return SetVariant(Variant, Value);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static float GetFloat(FBpVariant Variant)
	{
		return GetVariant<float>(Variant);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static FBpVariant MakeVariantFromFloat(const float Value)
	{
		FBpVariant variant;
		return SetVariant(variant, Value);
	}

	UFUNCTION(BlueprintCallable, Category="BpVariant")
	static FBpVariant SetDouble(UPARAM(ref)
	                            FBpVariant& Variant, const double Value)
	{
		return SetVariant(Variant, Value);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static double GetDouble(FBpVariant Variant)
	{
		return GetVariant<double>(Variant);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static FBpVariant MakeVariantFromDouble(const double Value)
	{
		FBpVariant variant;
		return SetVariant(variant, Value);
	}

	UFUNCTION(BlueprintCallable, Category="BpVariant")
	static FBpVariant SetName(UPARAM(ref)
	                          FBpVariant& Variant, const FName& Value)
	{
		return SetVariant(Variant, Value);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static FName GetName(FBpVariant Variant)
	{
		return GetVariant<FName>(Variant);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static FBpVariant MakeVariantFromName(const FName& Value)
	{
		FBpVariant variant;
		return SetVariant(variant, Value);
	}

	UFUNCTION(BlueprintCallable, Category="BpVariant")
	static FBpVariant SetString(UPARAM(ref)
	                            FBpVariant& Variant, const FString& Value)
	{
		return SetVariant(Variant, Value);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static FString GetString(FBpVariant Variant)
	{
		return GetVariant<FString>(Variant);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static FBpVariant MakeVariantFromString(const FString& Value)
	{
		return MakeFromFVariant(Value);
	}

	UFUNCTION(BlueprintCallable, Category="BpVariant")
	static FBpVariant SetText(UPARAM(ref)
	                          FBpVariant& Variant, const FText& Value)
	{
		return SetValue(Variant, Value);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static FText GetText(FBpVariant Variant)
	{
		return GetValue<FText>(Variant);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static FBpVariant MakeVariantFromText(const FText& Value)
	{
		FBpVariant variant;
		return SetValue(variant, Value);
	}

	UFUNCTION(BlueprintCallable, Category="BpVariant")
	static FBpVariant SetVector(UPARAM(ref)
	                            FBpVariant& Variant, const FVector& Value)
	{
		return SetVariant(Variant, Value);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static FVector GetVector(const FBpVariant& Variant)
	{
		return GetVariant<FVector>(Variant);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static FBpVariant MakeVariantFromVector(const FVector& Value)
	{
		return MakeFromFVariant(Value);
	}

	UFUNCTION(BlueprintCallable, Category="BpVariant")
	static FBpVariant SetRotator(UPARAM(ref)
	                             FBpVariant& Variant, const FRotator& Value)
	{
		return SetVariant(Variant, Value);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static FRotator GetRotator(const FBpVariant& Variant)
	{
		return GetVariant<FRotator>(Variant);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static FBpVariant MakeVariantFromRotator(const FRotator& Value)
	{
		return MakeFromFVariant(Value);
	}

	UFUNCTION(BlueprintCallable, Category="BpVariant")
	static FBpVariant SetTransform(UPARAM(ref)
	                               FBpVariant& Variant, const FTransform& Value)
	{
		return SetVariant(Variant, Value);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static FTransform GetTransform(const FBpVariant& Variant)
	{
		return GetVariant<FTransform>(Variant);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static FBpVariant MakeVariantFromTransform(const FTransform& Value)
	{
		return MakeFromFVariant(Value);
	}

	UFUNCTION(BlueprintCallable, Category="BpVariant")
	static FBpVariant SetStruct(UPARAM(ref)
	                            FBpVariant& Variant, const FInstancedStruct& Value)
	{
		return SetValue(Variant, Value);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static FInstancedStruct GetStruct(FBpVariant Variant)
	{
		return GetValue<FInstancedStruct>(Variant);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static FBpVariant MakeVariantFromStruct(const FInstancedStruct& Value)
	{
		FBpVariant variant;
		return SetValue(variant, Value);
	}

	UFUNCTION(BlueprintCallable, Category="BpVariant")
	static FBpVariant SetObject(UPARAM(ref)
	                            FBpVariant& Variant, UObject* Value)
	{
		return SetValue(Variant, Value);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static UObject* GetObject(const FBpVariant& Variant)
	{
		return *Variant.Data.TryGet<UObject*>();
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static FBpVariant MakeVariantFromObject(UObject* Value)
	{
		FBpVariant variant;
		return SetValue(variant, Value);
	}

	UFUNCTION(BlueprintCallable, Category="BpVariant")
	static FBpVariant SetClass(UPARAM(ref)
	                           FBpVariant& Variant, UClass* Value)
	{
		return SetValue(Variant, Value);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static UClass* GetClass(const FBpVariant& Variant)
	{
		return *Variant.Data.TryGet<UClass*>();
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static FBpVariant MakeVariantFromClass(UClass* Value)
	{
		FBpVariant variant;
		return SetValue(variant, Value);
	}

	UFUNCTION(BlueprintCallable, Category="BpVariant")
	static FBpVariant SetSoftObject(UPARAM(ref)
	                                FBpVariant& Variant, TSoftObjectPtr<UObject> Value)
	{
		return SetValue(Variant, Value);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static TSoftObjectPtr<UObject> GetSoftObject(const FBpVariant& Variant)
	{
		return *Variant.Data.TryGet<TSoftObjectPtr<UObject>>();
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static FBpVariant MakeVariantFromSoftObject(TSoftObjectPtr<UObject> Value)
	{
		FBpVariant variant;
		return SetValue(variant, Value);
	}

	UFUNCTION(BlueprintCallable, Category="BpVariant")
	static FBpVariant SetSoftClass(UPARAM(ref)
	                               FBpVariant& Variant, TSoftClassPtr<UObject> Value)
	{
		return SetValue(Variant, Value);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static TSoftClassPtr<UObject> GetSoftClass(const FBpVariant& Variant)
	{
		return *Variant.Data.TryGet<TSoftClassPtr<UObject>>();
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="BpVariant")
	static FBpVariant MakeVariantFromSoftClass(TSoftClassPtr<UObject> Value)
	{
		FBpVariant variant;
		return SetValue(variant, Value);
	}
};
