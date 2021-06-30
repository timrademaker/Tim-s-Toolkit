#pragma once

#include "Engine/DeveloperSettings.h"

#include "FeatureFlagSettings.generated.h"

class UFeatureFlagSettings;

DECLARE_EVENT(UFeatureFlagSettings, FFeatureFlagMapChanged);

UCLASS(Config=Game, defaultconfig, meta = (DisplayName = "Feature Flags"))
class TIMSTOOLKIT_API UFeatureFlagSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    static UFeatureFlagSettings* Get();

    FORCEINLINE FFeatureFlagMapChanged& OnFeatureFlagMapChanged()
    {
        return FeatureFlagMapChanged;
    }

public:
    /**
     * Key: The name of the feature
     * Value: Whether the feature is enabled or not
     */
    UPROPERTY(Config, EditAnywhere, Category = "Feature Flags")
    TMap<FName, bool> FeatureFlags;

    /** Whether features are considered enabled or not if they can't be found in the existing feature flags */
    UPROPERTY(Config, EditAnywhere, Category = "Feature Flags")
    bool bDefaultFeatureState;

private:
#if WITH_EDITOR
    virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
    static UFeatureFlagSettings* FeatureFlagSettingsSingleton;

    FFeatureFlagMapChanged FeatureFlagMapChanged;
};