// Copyright Epic Games, Inc. All Rights Reserved.

using System.Collections.Generic;
using System.Text;
using System.IO;
using EpicGames.Core;
using UnrealBuildTool;

public class BpValueBox : ModuleRules
{
	public BpValueBox(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[]
			{
				// ... add public include paths required here ...
			}
		);


		PrivateIncludePaths.AddRange(
			new string[]
			{
				// ... add other private include paths required here ...
			}
		);


		PublicDependencyModuleNames.AddRange(
			new[]
			{
				"Core"
				// ... add other public dependencies that you statically link with here ...
			}
		);


		PrivateDependencyModuleNames.AddRange(
			new[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore"
				// ... add private dependencies that you statically link with here ...	
			}
		);


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
		);
		// GenerateBoxedValueTypes();
	}

// 	private void GenerateBoxedValueTypes()
// 	{
// 		string reference = """
// 		                   UCLASS(Blueprintable)
// 		                   class BPVALUEBOX_API UBoxedStruct : public UObject, public IBoxedType
// 		                   {
// 		                   	GENERATED_BODY()
//
// 		                   public:
// 		                   	UPROPERTY(BlueprintReadWrite)
// 		                   	FInstancedStruct Value = FInstancedStruct();
//
// 		                   	virtual EValueType GetType_Implementation() override { return EValueType::Struct; }
//
// 		                   	UFUNCTION(BlueprintCallable, meta = ( DefaultToSelf = Context ), Category="BoxedValue")
// 		                   	static UBoxedStruct* BoxStruct(UObject* Context, const FInstancedStruct& Input)
// 		                   	{
// 		                   		return UBoxedValueStatics::BoxValue<UBoxedStruct>(Context, Input);
// 		                   	}
//
// 		                   	UFUNCTION(BlueprintPure, Category="BoxedValue")
// 		                   	static FInstancedStruct AsStruct(const TScriptInterface<IBoxedType>& Input)
// 		                   	{
// 		                   		return UBoxedValueStatics::GetValue<FInstancedStruct, UBoxedStruct>(Input);
// 		                   	}
// 		                   };
// 		                   """;
//
// 		List<(string type, string name)> types = new()
// 		{
// 			("FInstancedStruct", "Struct"),
// 		};
//
// 		foreach (var type in types)
// 		{
// 			string test = $@"
// UCLASS(Blueprintable)
// class BPVALUEBOX_API UBoxed{type.name} : public UObject, public IBoxedType
// {{
// 	GENERATED_BODY()
//
// public:
// 	UPROPERTY(BlueprintReadWrite)
// 	{type.type} Value = {type.type}();
//
// 	virtual EValueType GetType_Implementation() override {{ return EValueType::{type.name}; }}
//
// 	UFUNCTION(BlueprintCallable, meta = ( DefaultToSelf = Context ), Category=""BoxedValue"")
// 	static UBoxed{type.name}* Box{type.name}(UObject* Context, const {type.type}& Input)
// 	{{
// 		return UBoxedValueStatus::BoxValue<UBoxed{type.name}>(Context, Input);
// 	}}
//
// 	UFUNCTION(BlueprintPure, Category=""BoxedValue"")
// 	static {type.name} As{type.type}(const TScriptInterface<IBoxedType>& Input)
// 	{{
// 		return UBoxedValueStatus::GetValue<{type.type}, UBoxed{type.name}>(Input);
// 	}}
// }};
// ";
// 		}
// 	}
}