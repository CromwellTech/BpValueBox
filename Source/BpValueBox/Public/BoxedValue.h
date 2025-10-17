#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StructUtils/InstancedStruct.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ValueType.h"
#include "BoxedValue_Generated.h"
#include "BoxedValue.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UBoxedType : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BPVALUEBOX_API IBoxedType
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	EValueType GetType();
};

/* A struct wrapper around an IBoxedType. */
USTRUCT(BlueprintType)
struct FBoxedValue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TScriptInterface<IBoxedType> Value;
};

UCLASS()
class BPVALUEBOX_API UBoxedValueStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	template <typename TBoxType, typename TValueType>
	static TBoxType* BoxValue(UObject* Context, const TValueType& Value)
	{
		TBoxType* box = NewObject<TBoxType>(Context);
		box->Value = Value;
		return box;
	}

	template <typename TReturnType, typename TBoxedType>
	static TReturnType GetValue(const TScriptInterface<IBoxedType>& Value)
	{
		if (TBoxedType* box = Cast<TBoxedType>(Value.GetObject()))
		{
			return box->Value;
		}
		return TReturnType();
	}
};

UCLASS(Blueprintable)
class BPVALUEBOX_API UBoxedVariant : public UObject, public IBoxedType
{
	GENERATED_BODY()

public:
	FVariant Value = FVariant();

	virtual EValueType GetType_Implementation() override { return EValueType::String; }

	template <typename TVariant>
	static UBoxedVariant* BoxVariant(UObject* Context, TVariant Value)
	{
		UBoxedVariant* box = NewObject<UBoxedVariant>(Context);
		box->Value = Value;
		return box;
	}

	template <typename TReturnType>
	static TReturnType AsVariant(const TScriptInterface<IBoxedType>& Value)
	{
		if (UBoxedVariant* box = Cast<UBoxedVariant>(Value.GetObject()))
		{
			return box->Value.GetValue<TReturnType>();
		}
		return TReturnType();
	}

	UFUNCTION(BlueprintCallable, meta = ( DefaultToSelf = Context ), Category="BoxedValue")
	static UBoxedVariant* BoxBool(UObject* Context, const bool Value)
	{
		return BoxVariant(Context, Value);
	}

	UFUNCTION(BlueprintPure, Category="BoxedValue")
	static float AsFloat(const TScriptInterface<IBoxedType>& Value)
	{
		return AsVariant<float>(Value);
	}

	UFUNCTION(BlueprintCallable, meta = ( DefaultToSelf = Context ), Category="BoxedValue")
	static UBoxedVariant* BoxFloat(UObject* Context, const float Value)
	{
		return BoxVariant(Context, Value);
	}

	UFUNCTION(BlueprintPure, Category="BoxedValue")
	static int32 AsInt32(const TScriptInterface<IBoxedType>& Value)
	{
		return AsVariant<int32>(Value);
	}

	UFUNCTION(BlueprintCallable, meta = ( DefaultToSelf = Context ), Category="BoxedValue")
	static UBoxedVariant* BoxInt32(UObject* Context, const int32 Value)
	{
		return BoxVariant(Context, Value);
	}
};

UCLASS(Blueprintable)
class BPVALUEBOX_API UBoxedObject : public UObject, public IBoxedType
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	UObject* Value = nullptr;

	virtual EValueType GetType_Implementation() override { return EValueType::Object; }

	UFUNCTION(BlueprintCallable, meta = ( DefaultToSelf = Context ), Category="BoxedValue")
	static UBoxedObject* BoxObject(UObject* Context, UObject* Input)
	{
		return UBoxedValueStatics::BoxValue<UBoxedObject>(Context, Input);
	}

	UFUNCTION(BlueprintPure, Category="BoxedValue")
	static UObject* AsObject(const TScriptInterface<IBoxedType>& Input)
	{
		UBoxedObject* box = Cast<UBoxedObject>(Input.GetObject());
		if (box)
		{
			return box->Value;
		}
		return nullptr;
	}
};

UCLASS(Blueprintable)
class BPVALUEBOX_API UBoxedStruct : public UObject, public IBoxedType
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FInstancedStruct Value = FInstancedStruct();

	virtual EValueType GetType_Implementation() override { return EValueType::Struct; }

	UFUNCTION(BlueprintCallable, meta = ( DefaultToSelf = Context ), Category="BoxedValue")
	static UBoxedStruct* BoxStruct(UObject* Context, const FInstancedStruct& Input)
	{
		return UBoxedValueStatics::BoxValue<UBoxedStruct>(Context, Input);
	}

	UFUNCTION(BlueprintPure, Category="BoxedValue")
	static FInstancedStruct AsStruct(const TScriptInterface<IBoxedType>& Input)
	{
		return UBoxedValueStatics::GetValue<FInstancedStruct, UBoxedStruct>(Input);
	}
};

UCLASS(Blueprintable)
class BPVALUEBOX_API UBoxedBool : public UObject, public IBoxedType
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	bool Value = bool();

	virtual EValueType GetType_Implementation() override { return EValueType::Bool; }

	UFUNCTION(BlueprintCallable, meta = ( DefaultToSelf = Context ), Category="BoxedValue")
	static UBoxedBool* BoxBool(UObject* Context, bool Input)
	{
		return UBoxedValueStatics::BoxValue<UBoxedBool>(Context, Input);
	}

	UFUNCTION(BlueprintPure, Category="BoxedValue")
	static bool AsBool(const TScriptInterface<IBoxedType>& Input)
	{
		return UBoxedValueStatics::GetValue<bool, UBoxedBool>(Input);
	}
};

UCLASS(Blueprintable)
class BPVALUEBOX_API UBoxedByte : public UObject, public IBoxedType
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	uint8 Value = uint8();

	virtual EValueType GetType_Implementation() override { return EValueType::Byte; }

	UFUNCTION(BlueprintCallable, meta = ( DefaultToSelf = Context ), Category="BoxedValue")
	static UBoxedByte* BoxByte(UObject* Context, uint8 Input)
	{
		return UBoxedValueStatics::BoxValue<UBoxedByte>(Context, Input);
	}

	UFUNCTION(BlueprintPure, Category="BoxedValue")
	static uint8 AsByte(const TScriptInterface<IBoxedType>& Input)
	{
		return UBoxedValueStatics::GetValue<uint8, UBoxedByte>(Input);
	}
};

UCLASS(Blueprintable)
class BPVALUEBOX_API UBoxedInt32 : public UObject, public IBoxedType
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	int32 Value = int32();

	virtual EValueType GetType_Implementation() override { return EValueType::Int32; }

	UFUNCTION(BlueprintCallable, meta = ( DefaultToSelf = Context ), Category="BoxedValue")
	static UBoxedInt32* BoxInt32(UObject* Context, int32 Input)
	{
		return UBoxedValueStatics::BoxValue<UBoxedInt32>(Context, Input);
	}

	UFUNCTION(BlueprintPure, Category="BoxedValue")
	static int32 AsInt32(const TScriptInterface<IBoxedType>& Input)
	{
		return UBoxedValueStatics::GetValue<int32, UBoxedInt32>(Input);
	}
};

UCLASS(Blueprintable)
class BPVALUEBOX_API UBoxedInt64 : public UObject, public IBoxedType
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	int64 Value = int64();

	virtual EValueType GetType_Implementation() override { return EValueType::Int64; }

	UFUNCTION(BlueprintCallable, meta = ( DefaultToSelf = Context ), Category="BoxedValue")
	static UBoxedInt64* BoxInt64(UObject* Context, int64 Input)
	{
		return UBoxedValueStatics::BoxValue<UBoxedInt64>(Context, Input);
	}

	UFUNCTION(BlueprintPure, Category="BoxedValue")
	static int64 AsInt64(const TScriptInterface<IBoxedType>& Input)
	{
		return UBoxedValueStatics::GetValue<int64, UBoxedInt64>(Input);
	}
};

UCLASS(Blueprintable)
class BPVALUEBOX_API UBoxedFloat32 : public UObject, public IBoxedType
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	float Value = float();

	virtual EValueType GetType_Implementation() override { return EValueType::Float32; }

	UFUNCTION(BlueprintCallable, meta = ( DefaultToSelf = Context ), Category="BoxedValue")
	static UBoxedFloat32* BoxFloat32(UObject* Context, float Input)
	{
		return UBoxedValueStatics::BoxValue<UBoxedFloat32>(Context, Input);
	}

	UFUNCTION(BlueprintPure, Category="BoxedValue")
	static float AsFloat32(const TScriptInterface<IBoxedType>& Input)
	{
		return UBoxedValueStatics::GetValue<float, UBoxedFloat32>(Input);
	}
};

UCLASS(Blueprintable)
class BPVALUEBOX_API UBoxedFloat64 : public UObject, public IBoxedType
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	double Value = double();

	virtual EValueType GetType_Implementation() override { return EValueType::Float64; }

	UFUNCTION(BlueprintCallable, meta = ( DefaultToSelf = Context ), Category="BoxedValue")
	static UBoxedFloat64* BoxFloat64(UObject* Context, double Input)
	{
		return UBoxedValueStatics::BoxValue<UBoxedFloat64>(Context, Input);
	}

	UFUNCTION(BlueprintPure, Category="BoxedValue")
	static double AsFloat64(const TScriptInterface<IBoxedType>& Input)
	{
		return UBoxedValueStatics::GetValue<double, UBoxedFloat64>(Input);
	}
};

UCLASS(Blueprintable)
class BPVALUEBOX_API UBoxedName : public UObject, public IBoxedType
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FName Value = FName();

	virtual EValueType GetType_Implementation() override { return EValueType::Name; }

	UFUNCTION(BlueprintCallable, meta = ( DefaultToSelf = Context ), Category="BoxedValue")
	static UBoxedName* BoxName(UObject* Context, FName Input)
	{
		return UBoxedValueStatics::BoxValue<UBoxedName>(Context, Input);
	}

	UFUNCTION(BlueprintPure, Category="BoxedValue")
	static FName AsName(const TScriptInterface<IBoxedType>& Input)
	{
		return UBoxedValueStatics::GetValue<FName, UBoxedName>(Input);
	}
};

UCLASS(Blueprintable)
class BPVALUEBOX_API UBoxedString : public UObject, public IBoxedType
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FString Value = FString();

	virtual EValueType GetType_Implementation() override { return EValueType::String; }

	UFUNCTION(BlueprintCallable, meta = ( DefaultToSelf = Context ), Category="BoxedValue")
	static UBoxedString* BoxString(UObject* Context, const FString& Input)
	{
		return UBoxedValueStatics::BoxValue<UBoxedString>(Context, Input);
	}

	UFUNCTION(BlueprintPure, Category="BoxedValue")
	static FString AsString(const TScriptInterface<IBoxedType>& Input)
	{
		return UBoxedValueStatics::GetValue<FString, UBoxedString>(Input);
	}
};

UCLASS(Blueprintable)
class BPVALUEBOX_API UBoxedText : public UObject, public IBoxedType
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FText Value = FText();

	virtual EValueType GetType_Implementation() override { return EValueType::Text; }

	UFUNCTION(BlueprintCallable, meta = ( DefaultToSelf = Context ), Category="BoxedValue")
	static UBoxedText* BoxFloat32(UObject* Context, FText Input)
	{
		return UBoxedValueStatics::BoxValue<UBoxedText>(Context, Input);
	}

	UFUNCTION(BlueprintPure, Category="BoxedValue")
	static FText AsText(const TScriptInterface<IBoxedType>& Input)
	{
		return UBoxedValueStatics::GetValue<FText, UBoxedText>(Input);
	}
};

UCLASS(Blueprintable)
class BPVALUEBOX_API UBoxedVector : public UObject, public IBoxedType
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FVector Value = FVector();

	virtual EValueType GetType_Implementation() override { return EValueType::Vector; }

	UFUNCTION(BlueprintCallable, meta = ( DefaultToSelf = Context ), Category="BoxedValue")
	static UBoxedVector* BoxVector(UObject* Context, FVector Input)
	{
		return UBoxedValueStatics::BoxValue<UBoxedVector>(Context, Input);
	}

	UFUNCTION(BlueprintPure, Category="BoxedValue")
	static FVector AsVector(const TScriptInterface<IBoxedType>& Input)
	{
		return UBoxedValueStatics::GetValue<FVector, UBoxedVector>(Input);
	}
};

UCLASS(Blueprintable)
class BPVALUEBOX_API UBoxedRotator : public UObject, public IBoxedType
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FRotator Value = FRotator();

	virtual EValueType GetType_Implementation() override { return EValueType::Rotator; }

	UFUNCTION(BlueprintCallable, meta = ( DefaultToSelf = Context ), Category="BoxedValue")
	static UBoxedRotator* BoxRotator(UObject* Context, FRotator Input)
	{
		return UBoxedValueStatics::BoxValue<UBoxedRotator>(Context, Input);
	}

	UFUNCTION(BlueprintPure, Category="BoxedValue")
	static FRotator AsRotator(const TScriptInterface<IBoxedType>& Input)
	{
		return UBoxedValueStatics::GetValue<FRotator, UBoxedRotator>(Input);
	}
};

UCLASS(Blueprintable)
class BPVALUEBOX_API UBoxedTransform : public UObject, public IBoxedType
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FTransform Value = FTransform();

	virtual EValueType GetType_Implementation() override { return EValueType::Transform; }

	UFUNCTION(BlueprintCallable, meta = ( DefaultToSelf = Context ), Category="BoxedValue")
	static UBoxedTransform* BoxVector(UObject* Context, FTransform Input)
	{
		return UBoxedValueStatics::BoxValue<UBoxedTransform>(Context, Input);
	}

	UFUNCTION(BlueprintPure, Category="BoxedValue")
	static FTransform AsTransform(const TScriptInterface<IBoxedType>& Input)
	{
		return UBoxedValueStatics::GetValue<FTransform, UBoxedTransform>(Input);
	}
};
