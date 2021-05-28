// Fill out your copyright notice in the Description page of Project Settings.


#include "FeatureFlags/K2Node_IsFeatureEnabled.h"

#include "BlueprintNodeSpawner.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "EdGraphSchema_K2.h"
#include "KismetCompiler.h"

namespace FeatureFlagHelper
{
    const FName FeaturePinName = "Feature Name";
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
	
	// TODO: Make sure that the selected feature is valid
	const UEdGraphPin* featureNamePin = GetFeatureNamePin();
	if (!featureNamePin)
	{
		MessageLog.Error(TEXT("Missing FeatureName pin"), this);
		return;
	}
}


void UK2Node_IsFeatureEnabled::AllocateDefaultPins()
{
    // Add execution pins
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);
	UEdGraphPin* featureEnabledPin = CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);
	featureEnabledPin->PinFriendlyName = FText::FromString("Enabled");
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

	// TODO: Expand node to make sure the correct output is called
}

void UK2Node_IsFeatureEnabled::UpdateFeatureNameOptions()
{
	// Get feature name pin
	UEdGraphPin* featureNamePin = GetFeatureNamePin();
	featureNamePin->DefaultValue = "a";

	UEdGraph* Graph = GetGraph();
	Graph->NotifyGraphChanged();
}

UEdGraphPin* UK2Node_IsFeatureEnabled::GetFeatureNamePin() const
{
	UEdGraphPin* pin = FindPinChecked(FeatureFlagHelper::FeaturePinName);
	check(pin->Direction == EGPD_Input);
	return pin;
}
