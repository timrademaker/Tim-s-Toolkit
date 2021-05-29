#include "FeatureFlagFunctionLibrary.h"

#include "FeatureFlags.h"
#include "FeatureFlagSettings.h"

UFeatureFlagFunctionLibrary::UFeatureFlagFunctionLibrary(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

bool UFeatureFlagFunctionLibrary::IsFeatureEnabled(const FName& FeatureName)
{
    UFeatureFlagSettings* settings = UFeatureFlagSettings::Get();
    
    bool* featureIsEnabled = settings->FeatureFlags.Find(FeatureName);
    
    if(featureIsEnabled)
    {
        return *featureIsEnabled;
    }
    else
    {
        UE_LOG(LogFeatureFlags, Warning, TEXT("Unable to find feature flag for feature with name \"%s\""), *FeatureName.ToString());
        return settings->bDefaultFeatureState;
    }
}
