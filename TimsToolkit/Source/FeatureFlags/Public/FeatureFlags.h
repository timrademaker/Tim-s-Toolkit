#pragma once

#include "Modules/ModuleManager.h"

#include "IsFeatureEnabledPinFactory.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFeatureFlags, Log, All);

class FFeatureFlagsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedPtr<FIsFeatureEnabledPinFactory> IsFeatureEnabledPinFactory;
};
