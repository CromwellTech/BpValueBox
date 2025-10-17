- Boxing
  In my opinion, boxing is really the only way to go about doing this. Even doing the method described above of using
  Wildcards with Blueprint goes back to boxing as a UStruct* getting converted to an FProperty.

    - [FProperty](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/CoreUObject/UObject/FProperty)
      An FProperty is seemingly able to store any kind of value but it comes with a caveat of being very difficult to
      program for since it requires using a `UFUNCTION(CustomThunk)` in order to work with Blueprint which has a lot of
      undocumented information on it. Furthermore, I believe a lot of issues exist with using an FProperty with UObject*
      due to garbage collection. Regardless, this is a good option to look into as a universal way to box any type of
      value. It may be possible to internally store a `void*` inside of a class using an FProperty and then calling a
      getter function which returns the type casted out to the expected type? I'd personally expect `void*` to have many
      more issues regarding garbage collection since it's not able to register itself in Unreal's Reflection system.

    - UObject*
      This goes without saying but this would work for all UObject-inheriting types, which is every reference type in
      Unreal Engine. However, this would not work for structs or native types.

    - [FVariant](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Core/Misc/FVariant)
      An FVariant is able to store any one
      of [these](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Core/Misc/EVariantTypes) types.
      This covers all of the native types, so as far as those are concerned, this can be used but it cannot be used for
      FSTRUCTS.

    - [FInstancedStruct](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/CoreUObject/StructUtils/FInstancedStruct)
      An FInstancedStruct to store any kind of FSTRUCT. Internally, it uses FVariants for everything and stores each
      element of the FSTRUCT as its own FVariant. So, this answers the problem with the last one of not being able to
      support FSTRUCTS. One massive drawback to this is that, unfortunately, FInstancedStruct has been marked as
      deprecated which means that it is able to get nuked any update from now. However, since all of the code is
      source-available, simply copying that should be straightforward.

Using the three options listed above, (UObject*, FVariant, and FInstancedStruct), a simple IBoxedValue interface can be
made (or, heck, even a regular UBoxedValue) whose sole purpose is to store either a UObject*, an FVariant, or an
FInstancedStruct. It would be entirely reasonable to release this as its own standalone plugin, albeit since it'd be
such a straightforward plugin, I couldn't imagine charging any more than $5 for it. Anyways, separating that out into
its own plugin would be wise I believe.

Outside of the above, I think it would be worth trying to optionally make this work as a tree structure a la a
Javascript Object but not to be confused with a JSON Object.

Create a VariantTuple2-16 that is able to hold multiple FBpVariants

# BoxedValue

This is a class union. This takes up only as much memory as the type itself but is has reference semantics.

The IBoxedType interface is the primary type being used here.

An FBoxedValue is just a wrapper around the IBoxedType. In reality, this is not

# BpVariant

This is a struct union. This takes up more memory (32 bytes each), but it has value semantics.