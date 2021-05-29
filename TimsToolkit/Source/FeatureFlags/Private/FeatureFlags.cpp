// Copyright Epic Games, Inc. All Rights Reserved.

#include "FeatureFlags.h"

#define LOCTEXT_NAMESPACE "FFeatureFlagsModule"

void FFeatureFlagsModule::StartupModule()
{
	IsFeatureEnabledPinFactory = MakeShareable(new FIsFeatureEnabledPinFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(IsFeatureEnabledPinFactory);
	
}

void FFeatureFlagsModule::ShutdownModule()
{
	FEdGraphUtilities::UnregisterVisualPinFactory(IsFeatureEnabledPinFactory);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFeatureFlagsModule, FeatureFlags)
DEFINE_LOG_CATEGORY(LogFeatureFlags);
