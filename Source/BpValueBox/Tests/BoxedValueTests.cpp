#include "Misc/AutomationTest.h"
#include "BoxedValue.h"
#include "TestObject.h"
#include "ValueType.h"

IMPLEMENT_COMPLEX_AUTOMATION_TEST(BoxedValueTests, "Tests.BoxedValueTests",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

template <typename TBoxType, typename TInputType>
bool TestBoxedValue(FAutomationTestBase* Context, const TInputType& Input, const EValueType& ExpectedType)
{
	const TScriptInterface<IBoxedType> value = UBoxedValueStatics::BoxValue<TBoxType>(GetTransientPackage(), Input);
	const TInputType actual = UBoxedValueStatics::GetValue<TInputType, TBoxType>(value);

	const EValueType boxType = IBoxedType::Execute_GetType(value.GetObject());

	const bool typeCorrect = boxType == ExpectedType;
	const bool valueCorrect = actual == Input;

	Context->TestTrue(TEXT("BoxedValue type should be expected type"), typeCorrect);
	Context->TestTrue(TEXT("BoxedValue value should match the original value"), valueCorrect);

	return typeCorrect && valueCorrect;
}

bool TestBoxedText(FAutomationTestBase* Context, const FText& Input)
{
	const TScriptInterface<IBoxedType> value = UBoxedValueStatics::BoxValue<UBoxedText>(GetTransientPackage(), Input);
	const FText actual = UBoxedValueStatics::GetValue<FText, UBoxedText>(value);

	const EValueType boxType = IBoxedType::Execute_GetType(value.GetObject());

	const bool typeCorrect = boxType == EValueType::Text;
	const bool valueCorrect = actual.EqualTo(Input);

	Context->TestTrue(TEXT("BoxedValue type should be expected type"), typeCorrect);
	Context->TestTrue(TEXT("BoxedValue value should match the original value"), valueCorrect);

	return typeCorrect && valueCorrect;
}

bool TestBoxedTransform(FAutomationTestBase* Context, const FTransform& Input)
{
	const TScriptInterface<IBoxedType> value = UBoxedValueStatics::BoxValue<UBoxedTransform>(
		GetTransientPackage(), Input);
	const FTransform actual = UBoxedValueStatics::GetValue<FTransform, UBoxedTransform>(value);

	const EValueType boxType = IBoxedType::Execute_GetType(value.GetObject());

	const bool typeCorrect = boxType == EValueType::Transform;
	const bool valueCorrect = actual.Equals(Input);

	Context->TestTrue(TEXT("BoxedValue type should be expected type"), typeCorrect);
	Context->TestTrue(TEXT("BoxedValue value should match the original value"), valueCorrect);

	return typeCorrect && valueCorrect;
}

template <typename T>
bool TestBoxedStruct(FAutomationTestBase* Context, const T& Input)
{
	const TScriptInterface<IBoxedType> value = UBoxedValueStatics::BoxValue<UBoxedStruct>(
		GetTransientPackage(), FInstancedStruct::Make(Input));
	const FInstancedStruct actual = UBoxedValueStatics::GetValue<FInstancedStruct, UBoxedStruct>(value);

	const EValueType boxType = IBoxedType::Execute_GetType(value.GetObject());

	const bool typeCorrect = boxType == EValueType::Struct;
	const bool valueCorrect = actual.Get<T>() == Input;

	Context->TestTrue(TEXT("BoxedValue type should be expected type"), typeCorrect);
	Context->TestTrue(TEXT("BoxedValue value should match the original value"), valueCorrect);

	return typeCorrect && valueCorrect;
}

bool TestBoxedObject(FAutomationTestBase* Context, UObject* Input)
{
	const TScriptInterface<IBoxedType> value = UBoxedValueStatics::BoxValue<UBoxedObject>(GetTransientPackage(), Input);
	const UObject* actual = UBoxedValueStatics::GetValue<UObject*, UBoxedObject>(value);

	const EValueType boxType = IBoxedType::Execute_GetType(value.GetObject());

	const bool typeCorrect = boxType == EValueType::Object;
	const bool valueCorrect = actual == Input;

	Context->TestTrue(TEXT("BoxedValue type should be expected type"), typeCorrect);
	Context->TestTrue(TEXT("BoxedValue value should match the original value"), valueCorrect);

	return typeCorrect && valueCorrect;
}

bool TestBoxCanBeChanged(FAutomationTestBase* Context)
{
	constexpr bool input1 = true;
	TScriptInterface<IBoxedType> value = UBoxedBool::BoxBool(GetTransientPackage(), input1);

	const bool initialTypeCorrect = IBoxedType::Execute_GetType(value.GetObject()) == EValueType::Bool;
	const bool initialValueCorrect = UBoxedBool::AsBool(value) == input1;

	Context->TestTrue(TEXT("Initial variant type should be expected type"), initialTypeCorrect);
	Context->TestTrue(TEXT("Initial variant value should match the original value"), initialValueCorrect);

	TScriptInterface<IBoxedType> copyValue = value;

	const FTransform input2 = FTransform(FVector(1));
	value = UBoxedTransform::BoxVector(GetTransientPackage(), input2);

	const bool finalTypeCorrect = IBoxedType::Execute_GetType(value.GetObject()) == EValueType::Transform;
	const bool finalValueCorrect = UBoxedTransform::AsTransform(value).Equals(input2);

	Context->TestTrue(TEXT("Final variant type should be expected type"), finalTypeCorrect);
	Context->TestTrue(TEXT("Final variant value should match the original value"), finalValueCorrect);

	const bool copyTypeCorrect = IBoxedType::Execute_GetType(copyValue.GetObject()) == EValueType::Bool;
	const bool copyValueCorrect = UBoxedBool::AsBool(copyValue) == input1;

	Context->TestTrue(TEXT("Copy variant type should be expected type"), copyTypeCorrect);
	Context->TestTrue(TEXT("Copy variant value should match the original value"), copyValueCorrect);

	return initialTypeCorrect && initialValueCorrect && finalTypeCorrect && finalValueCorrect && copyTypeCorrect &&
		copyValueCorrect;
}

const FString BoxedValueTests_BoxedBool = TEXT("BoxedValueTests_BoxedBool");
const FString BoxedValueTests_BoxedByte = TEXT("BoxedValueTests_BoxedByte");
const FString BoxedValueTests_BoxedInt32 = TEXT("BoxedValueTests_BoxedInt32");
const FString BoxedValueTests_BoxedInt64 = TEXT("BoxedValueTests_BoxedInt64");
const FString BoxedValueTests_BoxedFloat32 = TEXT("BoxedValueTests_BoxedFloat32");
const FString BoxedValueTests_BoxedFloat64 = TEXT("BoxedValueTests_BoxedFloat64");
const FString BoxedValueTests_BoxedName = TEXT("BoxedValueTests_BoxedName");
const FString BoxedValueTests_BoxedString = TEXT("BoxedValueTests_BoxedString");
const FString BoxedValueTests_BoxedText = TEXT("BoxedValueTests_BoxedText");
const FString BoxedValueTests_BoxedVector = TEXT("BoxedValueTests_BoxedVector");
const FString BoxedValueTests_BoxedRotator = TEXT("BoxedValueTests_BoxedRotator");
const FString BoxedValueTests_BoxedTransform = TEXT("BoxedValueTests_BoxedTransform");
const FString BoxedValueTests_BoxedStruct = TEXT("BoxedValueTests_BoxedStruct");
const FString BoxedValueTests_BoxedObject = TEXT("BoxedValueTests_BoxedObject");
const FString BoxedValueTests_BoxCanBeChanged = TEXT("BoxedValueTests_BoxCanBeChanged");

void BoxedValueTests::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	TArray<FString> tests =
	{
		BoxedValueTests_BoxedBool,
		BoxedValueTests_BoxedByte,
		BoxedValueTests_BoxedString,
		BoxedValueTests_BoxedInt32,
		BoxedValueTests_BoxedInt64,
		BoxedValueTests_BoxedFloat32,
		BoxedValueTests_BoxedFloat64,
		BoxedValueTests_BoxedName,
		BoxedValueTests_BoxedString,
		BoxedValueTests_BoxedText,
		BoxedValueTests_BoxedVector,
		BoxedValueTests_BoxedRotator,
		BoxedValueTests_BoxedTransform,
		BoxedValueTests_BoxedStruct,
		BoxedValueTests_BoxedObject,
		BoxedValueTests_BoxCanBeChanged,
	};

	for (const FString& test : tests)
	{
		OutBeautifiedNames.Add(test);
		OutTestCommands.Add(test);
	}
}

bool BoxedValueTests::RunTest(const FString& Parameters)
{
	TMap<FString, TFunction<bool()>> tests =
	{
		{
			BoxedValueTests_BoxedBool,
			[this]() { return TestBoxedValue<UBoxedBool>(this, true, EValueType::Bool); }
		},
		{
			BoxedValueTests_BoxedByte,
			[this]() { return TestBoxedValue<UBoxedByte>(this, static_cast<uint8>(1), EValueType::Byte); }
		},
		{
			BoxedValueTests_BoxedInt32,
			[this]() { return TestBoxedValue<UBoxedInt32>(this, static_cast<int32>(1), EValueType::Int32); }
		},
		{
			BoxedValueTests_BoxedInt64,
			[this]() { return TestBoxedValue<UBoxedInt64>(this, static_cast<int64>(1), EValueType::Int64); }
		},
		{
			BoxedValueTests_BoxedFloat32,
			[this]() { return TestBoxedValue<UBoxedFloat32>(this, static_cast<float>(1), EValueType::Float32); }
		},
		{
			BoxedValueTests_BoxedFloat64,
			[this]() { return TestBoxedValue<UBoxedFloat64>(this, static_cast<double>(1), EValueType::Float64); }
		},
		{
			BoxedValueTests_BoxedName,
			[this]() { return TestBoxedValue<UBoxedName>(this, FName(FGuid::NewGuid().ToString()), EValueType::Name); }
		},
		{
			BoxedValueTests_BoxedString,
			[this]() { return TestBoxedValue<UBoxedString>(this, FGuid::NewGuid().ToString(), EValueType::String); }
		},
		{
			BoxedValueTests_BoxedText,
			[this]() { return TestBoxedText(this, FText::FromString(FGuid::NewGuid().ToString())); }
		},
		{
			BoxedValueTests_BoxedVector,
			[this]() { return TestBoxedValue<UBoxedVector>(this, FVector(1), EValueType::Vector); }
		},
		{
			BoxedValueTests_BoxedRotator,
			[this]() { return TestBoxedValue<UBoxedRotator>(this, FRotator(1), EValueType::Rotator); }
		},
		{
			BoxedValueTests_BoxedTransform,
			[this]() { return TestBoxedTransform(this, FTransform(FVector(1))); }
		},
		{
			BoxedValueTests_BoxedStruct,
			[this]() { return TestBoxedStruct(this, FVector(1)); }
		},
		{
			BoxedValueTests_BoxedObject,
			[this]() { return TestBoxedObject(this, NewObject<UTestObject>()); }
		},
		{
			BoxedValueTests_BoxCanBeChanged,
			[this]() { return TestBoxCanBeChanged(this); }
		},
	};

	if (tests.Contains(Parameters))
	{
		return tests[Parameters]();
	}
	return true;
}
