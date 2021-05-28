#pragma once

#include "Modules/ModuleManager.h"

#include "FeatureFlags/IsFeatureEnabledPinFactory.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTimsToolkit, Log, All);

class FTimsToolkitModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedPtr<FIsFeatureEnabledPinFactory> IsFeatureEnabledPinFactory;
};
