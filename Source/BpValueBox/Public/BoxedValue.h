#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ValueType.h"
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
