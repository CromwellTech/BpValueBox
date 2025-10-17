#include "Misc/AutomationTest.h"
#include "BpVariant.h"
#include "ValueType.h"
#include "TestObject.h"

IMPLEMENT_COMPLEX_AUTOMATION_TEST(BpVariantTests, "Tests.BpVariantTests",
								  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

template <typename T>
bool TestFVariant(FAutomationTestBase* Context, const T& Input, const EValueType& ExpectedType)
{
	FVariant variant = Input;
	const FBpVariant value = UBpVariantStatics::MakeFromFVariant(variant);
	const T actual = UBpVariantStatics::GetVariant<T>(value);

	const bool typeCorrect = UBpVariantStatics::GetType(value) == ExpectedType;
	const bool valueCorrect = actual == Input;

	Context->TestTrue(TEXT("Variant type should be ExpectedType"), typeCorrect);
	Context->TestTrue(TEXT("Variant value should match the original value"), valueCorrect);

	return typeCorrect && valueCorrect;
}

bool TestTextVariant(FAutomationTestBase* Context, const FText& Input)
{
	const FBpVariant value = UBpVariantStatics::MakeVariantFromText(Input);
	const FText actual = UBpVariantStatics::GetText(value);

	const bool typeCorrect = UBpVariantStatics::GetType(value) == EValueType::Text;
	const bool valueCorrect = actual.EqualTo(Input);

	Context->TestTrue(TEXT("Variant type should be expected type"), typeCorrect);
	Context->TestTrue(TEXT("Variant value should match the original value"), valueCorrect);

	return typeCorrect && valueCorrect;
}

bool TestFTransformVariant(FAutomationTestBase* Context, const FTransform& Input)
{
	const FBpVariant value = UBpVariantStatics::MakeVariantFromTransform(Input);
	const FTransform actual = UBpVariantStatics::GetTransform(value);

	const bool typeCorrect = UBpVariantStatics::GetType(value) == EValueType::Transform;
	const bool valueCorrect = actual.Equals(Input);

	Context->TestTrue(TEXT("Variant type should be expected type"), typeCorrect);
	Context->TestTrue(TEXT("Variant value should match the original value"), valueCorrect);

	return typeCorrect && valueCorrect;
}

template <typename T>
bool TestStructVariant(FAutomationTestBase* Context, const T& Input)
{
	const FBpVariant value = UBpVariantStatics::MakeVariantFromStruct(FInstancedStruct::Make(Input));
	const FInstancedStruct actual = UBpVariantStatics::GetStruct(value);

	const bool typeCorrect = UBpVariantStatics::GetType(value) == EValueType::Struct;
	const bool valueCorrect = actual.Get<T>() == Input;

	Context->TestTrue(TEXT("Variant type should be expected type"), typeCorrect);
	Context->TestTrue(TEXT("Variant value should match the original value"), valueCorrect);

	return typeCorrect && valueCorrect;
}

bool TestObjectVariant(FAutomationTestBase* Context, UObject* Input)
{
	const FBpVariant value = UBpVariantStatics::MakeVariantFromObject(Input);
	const UObject* actual = UBpVariantStatics::GetObject(value);

	const bool typeCorrect = UBpVariantStatics::GetType(value) == EValueType::Object;
	const bool valueCorrect = actual == Input;

	Context->TestTrue(TEXT("Variant type should be expected type"), typeCorrect);
	Context->TestTrue(TEXT("Variant value should match the original value"), valueCorrect);

	return typeCorrect && valueCorrect;
}

bool TestVariantCanBeChanged(FAutomationTestBase* Context)
{
	const FVariant input1 = true;
	FBpVariant value = UBpVariantStatics::MakeFromFVariant(input1);

	const bool initialTypeCorrect = UBpVariantStatics::GetType(value) == EValueType::Bool;
	const bool initialValueCorrect = UBpVariantStatics::GetVariant<bool>(value) == input1.GetValue<bool>();

	Context->TestTrue(TEXT("Initial variant type should be expected type"), initialTypeCorrect);
	Context->TestTrue(TEXT("Initial variant value should match the original value"), initialValueCorrect);

	const FBpVariant copyValue = value;

	const FTransform input2 = FTransform(FVector(1));
	value = UBpVariantStatics::MakeVariantFromTransform(input2);

	const bool finalTypeCorrect = UBpVariantStatics::GetType(value) == EValueType::Transform;
	const bool finalValueCorrect = UBpVariantStatics::GetTransform(value).Equals(input2);

	Context->TestTrue(TEXT("Final variant type should be expected type"), finalTypeCorrect);
	Context->TestTrue(TEXT("Final variant value should match the original value"), finalValueCorrect);

	const bool copyTypeCorrect = UBpVariantStatics::GetType(copyValue) == EValueType::Bool;
	const bool copyValueCorrect = UBpVariantStatics::GetVariant<bool>(copyValue) == input1.GetValue<bool>();

	Context->TestTrue(TEXT("Copy variant type should be expected type"), copyTypeCorrect);
	Context->TestTrue(TEXT("Copy variant value should match the original value"), copyValueCorrect);

	return initialTypeCorrect && initialValueCorrect && finalTypeCorrect && finalValueCorrect && copyTypeCorrect &&
		copyValueCorrect;
}

const FString BpVariantTests_Bool = TEXT("BpVariantTests_Bool");
const FString BpVariantTests_Byte = TEXT("BpVariantTests_Byte");
const FString BpVariantTests_Int32 = TEXT("BpVariantTests_Int32");
const FString BpVariantTests_Int64 = TEXT("BpVariantTests_Int64");
const FString BpVariantTests_Float32 = TEXT("BpVariantTests_Float32");
const FString BpVariantTests_Float64 = TEXT("BpVariantTests_Float64");
const FString BpVariantTests_FName = TEXT("BpVariantTests_FName");
const FString BpVariantTests_FString = TEXT("BpVariantTests_FString");
const FString BpVariantTests_FText = TEXT("BpVariantTests_FText");
const FString BpVariantTests_Vector = TEXT("BpVariantTests_Vector");
const FString BpVariantTests_Rotator = TEXT("BpVariantTests_Rotator");
const FString BpVariantTests_Transform = TEXT("BpVariantTests_Transform");
const FString BpVariantTests_Struct = TEXT("BpVariantTests_Struct");
const FString BpVariantTests_Object = TEXT("BpVariantTests_Object");
const FString BpVariantTests_VariantCanBeChanged = TEXT("BpVariantTests_VariantCanBeChanged");

void BpVariantTests::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	TArray<FString> tests =
	{
		BpVariantTests_Bool,
		BpVariantTests_Byte,
		BpVariantTests_Int32,
		BpVariantTests_Int64,
		BpVariantTests_Float32,
		BpVariantTests_Float64,
		BpVariantTests_FName,
		BpVariantTests_FString,
		BpVariantTests_FText,
		BpVariantTests_Vector,
		BpVariantTests_Rotator,
		BpVariantTests_Transform,
		BpVariantTests_Struct,
		BpVariantTests_Object,
		BpVariantTests_VariantCanBeChanged,
	};

	for (const FString& test : tests)
	{
		OutBeautifiedNames.Add(test);
		OutTestCommands.Add(test);
	}
}

bool BpVariantTests::RunTest(const FString& Parameters)
{
	TMap<FString, TFunction<bool()>> tests =
	{
		{
			BpVariantTests_Bool,
			[this]() { return TestFVariant(this, true, EValueType::Bool); }
		},
		{
			BpVariantTests_Byte,
			[this]() { return TestFVariant(this, static_cast<uint8>(1), EValueType::Byte); }
		},
		{
			BpVariantTests_Int32,
			[this]() { return TestFVariant(this, static_cast<int32>(1), EValueType::Int32); }
		},
		{
			BpVariantTests_Int64,
			[this]() { return TestFVariant(this, static_cast<int64>(1), EValueType::Int64); }
		},
		{
			BpVariantTests_Float32,
			[this]() { return TestFVariant(this, static_cast<float>(1), EValueType::Float32); }
		},
		{
			BpVariantTests_Float64,
			[this]() { return TestFVariant(this, static_cast<double>(1), EValueType::Float64); }
		},
		{
			BpVariantTests_FName,
			[this]() { return TestFVariant(this, FName(FGuid::NewGuid().ToString()), EValueType::Name); }
		},
		{
			BpVariantTests_FString,
			[this]() { return TestFVariant(this, FGuid::NewGuid().ToString(), EValueType::String); }
		},
		{
			BpVariantTests_FText,
			[this]() { return TestTextVariant(this, FText::FromString(FGuid::NewGuid().ToString())); }
		},
		{
			BpVariantTests_Vector,
			[this]() { return TestFVariant(this, FVector(1, 1, 1), EValueType::Vector); }
		},
		{
			BpVariantTests_Rotator,
			[this]() { return TestFVariant(this, FRotator(1, 1, 1), EValueType::Rotator); }
		},
		{
			BpVariantTests_Transform,
			[this]() { return TestFTransformVariant(this, FTransform(FVector(1, 1, 1))); }
		},
		{
			BpVariantTests_Struct,
			[this]() { return TestStructVariant(this, FVector(1, 1, 1)); }
		},
		{
			BpVariantTests_Object,
			[this]() { return TestObjectVariant(this, NewObject<UTestObject>()); }
		},
		{
			BpVariantTests_VariantCanBeChanged,
			[this]() { return TestVariantCanBeChanged(this); }
		},
	};

	if (tests.Contains(Parameters))
	{
		return tests[Parameters]();
	}
	return true;
}
