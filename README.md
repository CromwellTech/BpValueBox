# BlueprintValueBox

A simple plugin for Unreal Engine to allow for boxing any value type within Blueprint,
akin to `std::any` in C++, `Object` in C#/Java, `Variant` in GDScript, an empty interface in Golang, etc.

I made this as a library for another project I was working on but I figured it might be useful for others as well.

If you are looking for something with more support, I'd recommend looking
into [AnyValue](https://www.fab.com/listings/242b2e28-db6b-4e69-9744-067b4e171a0f).
It's a paid addon but it is more mature and has more features.

# Supported Types

This supports a good amount of Blueprint types, including:

- `FVariant`
- `UObject*` (covers all Objects, Actors, Components; any instance of a class works)
- `FInstancedStruct` (covers nearly all Structs that are exposed to Blueprint)
- `bool`
- `byte` (works with all Enums)
- `int32`
- `int64`
- `float`
- `double`
- `FName`
- `FString`
- `FText`
- `FVector`
- `FRotator`
- `FTransform`

I'm not sure if any other types will be necessary to add but if so,
they can be added as needed easily through the source generators.

# Usage

There are two different types of classes you can use depending on if you're wanting value or reference semantics:

1. `BoxedValue`
    - A group of classes that implement `IBoxedType` so you'd likely want to pass around the `IBoxedType`
    - This is more memory efficient, but requires a heap allocation and requires casting
    - To use this in C++, you'll likely want to `#include "BoxedValue.h"` and `#include "BoxedValue_Generated.h"
2. `BpVariant`
    - A struct that is a union of all the supported types
    - Doesn't incur a heap allocation and doesn't require casting, but consumes more memory (56 bytes)
-- better for hot code paths and tight loops