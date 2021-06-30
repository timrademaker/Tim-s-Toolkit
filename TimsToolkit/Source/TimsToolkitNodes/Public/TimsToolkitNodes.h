#pragma once

#include "Modules/ModuleManager.h"

#include "IsFeatureEnabledPinFactory.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTimsToolkitNodes, Log, All);

class FTimsToolkitNodesModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedPtr<FIsFeatureEnabledPinFactory> IsFeatureEnabledPinFactory;
};
