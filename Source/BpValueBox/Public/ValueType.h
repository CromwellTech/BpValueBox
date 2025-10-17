#pragma once

#include "CoreMinimal.h"
#include "ValueType.generated.h"

UENUM(BlueprintType)
enum class EValueType : uint8
{
	Bool,
	Byte,
	Int32,
	Int64,
	Float32,
	Float64,
	Name,
	String,
	Text,
	Vector,
	Rotator,
	Transform,
	Struct,
	Object,
	None,
	// May need to add a UObjectSoftPointer, UClass, and UClassSoftPointer
};
