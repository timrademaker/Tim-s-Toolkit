// Fill out your copyright notice in the Description page of Project Settings.

#include "TimsToolkitNodes.h"

#define LOCTEXT_NAMESPACE "FTimsToolkitNodesModule"

void FTimsToolkitNodesModule::StartupModule()
{
	IsFeatureEnabledPinFactory = MakeShareable(new FIsFeatureEnabledPinFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(IsFeatureEnabledPinFactory);
	
}

void FTimsToolkitNodesModule::ShutdownModule()
{
	FEdGraphUtilities::UnregisterVisualPinFactory(IsFeatureEnabledPinFactory);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTimsToolkitNodesModule, TimsToolkitNodes)
DEFINE_LOG_CATEGORY(LogTimsToolkitNodes);
