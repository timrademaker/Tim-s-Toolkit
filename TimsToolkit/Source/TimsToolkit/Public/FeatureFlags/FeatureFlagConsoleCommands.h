#pragma once

DECLARE_EVENT_TwoParams(FFeatureFlagConsoleCommands, FFeatureStateChanged, FName /* FeatureName */, bool /* bNewEnabled */);

class FFeatureFlagConsoleCommands
{
#if !UE_BUILD_SHIPPING
public:
    static void SetFeatureEnabled(const TArray<FString>& Args);
#endif

public:
    static FFeatureStateChanged OnFeatureStateChanged;
};
