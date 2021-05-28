// Copyright Epic Games, Inc. All Rights Reserved.

#include "TimsToolkit.h"

#define LOCTEXT_NAMESPACE "FTimsToolkitModule"

void FTimsToolkitModule::StartupModule()
{
	IsFeatureEnabledPinFactory = MakeShareable(new FIsFeatureEnabledPinFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(IsFeatureEnabledPinFactory);
	
}

void FTimsToolkitModule::ShutdownModule()
{
	FEdGraphUtilities::UnregisterVisualPinFactory(IsFeatureEnabledPinFactory);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTimsToolkitModule, TimsToolkit)
DEFINE_LOG_CATEGORY(LogTimsToolkit);
