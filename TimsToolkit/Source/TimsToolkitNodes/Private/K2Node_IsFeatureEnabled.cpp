// Fill out your copyright notice in the Description page of Project Settings.


#include "K2Node_IsFeatureEnabled.h"
#include "FeatureFlags/FeatureFlagSettings.h"
#include "FeatureFlags/FeatureFlagFunctionLibrary.h"

#include "BlueprintNodeSpawner.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "EdGraphSchema_K2.h"
#include "KismetCompiler.h"
#include "Kismet/BlueprintMapLibrary.h"
#include "K2Node_IfThenElse.h"
#include "K2Node_CallFunction.h"

namespace FeatureFlagHelper
{
    const FName FeaturePinName = "Feature Name";
	const FName FeatureEnabledPinName = "Enabled";
	const FName FeatureDisabledPinName = "Disabled";
}

void UK2Node_IsFeatureEnabled::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	// I copied this from UK2Node_GetDataTableRow, what are you going to do about it?

	UClass* actionKey = GetClass();
	
	if (ActionRegistrar.IsOpenForRegistration(actionKey))
	{
		UBlueprintNodeSpawner* nodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(nodeSpawner != nullptr);

		ActionRegistrar.AddBlueprintAction(actionKey, nodeSpawner);
	}
}

FText UK2Node_IsFeatureEnabled::GetMenuCategory() const
{
	return FText::FromString("Tim's Toolkit|Feature Flags");
}

void UK2Node_IsFeatureEnabled::EarlyValidation(FCompilerResultsLog& MessageLog) const
{
	Super::EarlyValidation(MessageLog);
	
	const UEdGraphPin* featureNamePin = GetFeatureNamePin();
	if (!featureNamePin)
	{
		MessageLog.Error(TEXT("Missing FeatureName pin"), this);
		return;
	}

	const UEdGraphPin* execPin = GetExecPin();

	if (featureNamePin->LinkedTo.Num() > 0 && execPin != nullptr && execPin->LinkedTo.Num() > 0)
	{	
		const FString featureName = featureNamePin->LinkedTo[0]->GetDefaultAsString();
		UFeatureFlagSettings* settings = UFeatureFlagSettings::Get();
		if (!settings->FeatureFlags.Contains(FName(featureName)))
		{
			const FString msg = FText::Format(
				FText::FromString("'{0}' is not a valid feature name!"),
				FText::FromString(featureName)
			).ToString();

			MessageLog.Error(*msg, this);
			return;
		}
	}
}

void UK2Node_IsFeatureEnabled::AllocateDefaultPins()
{
    // Add execution pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	UEdGraphPin* featureEnabledPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
	featureEnabledPin->PinFriendlyName = FText::FromName(FeatureFlagHelper::FeatureEnabledPinName);
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, FeatureFlagHelper::FeatureDisabledPinName);

	// Feature Name pin
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, FeatureFlagHelper::FeaturePinName);

	Super::AllocateDefaultPins();
}

FText UK2Node_IsFeatureEnabled::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (TitleType == ENodeTitleType::MenuTitle)
	{
		return FText::FromString("Is Feature Enabled");
	}
	else
	{
		return NSLOCTEXT("TimsToolkit", "IsFeatureEnabledNodeTitle", "Is Feature Enabled");
	}
}

void UK2Node_IsFeatureEnabled::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	UEdGraphPin* originalFeatureNamePin = GetFeatureNamePin();

	// Create a node to check if the selected feature is enabled
	const FName isFeatureEnabledFunctionName = GET_FUNCTION_NAME_CHECKED(UFeatureFlagFunctionLibrary, IsFeatureEnabled);
	UK2Node_CallFunction* isFeatureEnabledFunction = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	isFeatureEnabledFunction->FunctionReference.SetExternalMember(isFeatureEnabledFunctionName, UFeatureFlagFunctionLibrary::StaticClass());
	isFeatureEnabledFunction->AllocateDefaultPins();
	// No need to connect exec pin to isFeatureEnabledFunction, as it's a pure node

	// Move whatever is connected to the FeatureName pin to the find-function
	UEdGraphPin* featureNamePin = isFeatureEnabledFunction->FindPinChecked(TEXT("FeatureName"));
	if (originalFeatureNamePin->LinkedTo.Num() > 0)
	{
		// Copy connection
		CompilerContext.MovePinLinksToIntermediate(*originalFeatureNamePin, *featureNamePin);
	}
	else
	{
		// Copy literal
		featureNamePin->DefaultValue = originalFeatureNamePin->DefaultValue;
	}

	// Create a branch node
	UK2Node_IfThenElse* branchNode = CompilerContext.SpawnIntermediateNode<UK2Node_IfThenElse>(this, SourceGraph);
	branchNode->AllocateDefaultPins();
	CompilerContext.MovePinLinksToIntermediate(*GetExecPin(), *(branchNode->GetExecPin()));
	
	// Connect condition pin
	UEdGraphPin* featureEnabledReturnPin = isFeatureEnabledFunction->GetReturnValuePin();
	featureEnabledReturnPin->MakeLinkTo(branchNode->GetConditionPin());

	// Hook up output pins
	UEdGraphPin* featureEnabledPin = GetFeatureEnabledPin();
	UEdGraphPin* featureDisabledPin = GetFeatureDisabledPin();
	
	CompilerContext.MovePinLinksToIntermediate(*featureEnabledPin, *(branchNode->GetThenPin()));
	CompilerContext.MovePinLinksToIntermediate(*featureDisabledPin, *(branchNode->GetElsePin()));

	BreakAllNodeLinks();
}

void UK2Node_IsFeatureEnabled::UpdateFeatureNameOptions()
{
	UEdGraph* Graph = GetGraph();
	Graph->NotifyGraphChanged();
}

UEdGraphPin* UK2Node_IsFeatureEnabled::GetFeatureNamePin() const
{
	UEdGraphPin* pin = FindPinChecked(FeatureFlagHelper::FeaturePinName);
	check(pin->Direction == EGPD_Input);
	return pin;
}

UEdGraphPin* UK2Node_IsFeatureEnabled::GetFeatureEnabledPin()
{
	UEdGraphPin* pin = FindPinChecked(UEdGraphSchema_K2::PN_Then);
	check(pin->Direction == EGPD_Output);
	return pin;
}

UEdGraphPin* UK2Node_IsFeatureEnabled::GetFeatureDisabledPin()
{
	UEdGraphPin* pin = FindPinChecked(FeatureFlagHelper::FeatureDisabledPinName);
	check(pin->Direction == EGPD_Output);
	return pin;
}
