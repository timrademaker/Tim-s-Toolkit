#include "FeatureFlags/FeatureFlagSettings.h"

#include "UObject/UObjectGlobals.h"

UFeatureFlagSettings* UFeatureFlagSettings::FeatureFlagSettingsSingleton = nullptr;


UFeatureFlagSettings* UFeatureFlagSettings::Get()
{
    if (!FeatureFlagSettingsSingleton)
    {
        static const TCHAR* settingsContainerName = TEXT("FeatureFlagSettingsContainer");
        FeatureFlagSettingsSingleton = FindObject<UFeatureFlagSettings>(GetTransientPackage(), settingsContainerName);

        if (!FeatureFlagSettingsSingleton)
        {
            FeatureFlagSettingsSingleton = NewObject<UFeatureFlagSettings>(GetTransientPackage(), UFeatureFlagSettings::StaticClass(), settingsContainerName);
            FeatureFlagSettingsSingleton->AddToRoot();
        }
    }

    return FeatureFlagSettingsSingleton;
}
