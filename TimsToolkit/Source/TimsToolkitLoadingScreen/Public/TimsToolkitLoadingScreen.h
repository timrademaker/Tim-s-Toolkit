#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"


class FTimsToolkitLoadingScreenModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void BeginLoadingScreen();

	void OnLevelLoaded(UWorld* LoadedWorld);

private:
	UWorld* CurrentWorld = nullptr;
};
