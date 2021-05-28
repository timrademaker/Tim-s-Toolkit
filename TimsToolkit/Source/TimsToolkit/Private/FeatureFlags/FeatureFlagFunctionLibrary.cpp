#include "FeatureFlags/FeatureFlagFunctionLibrary.h"

#include "FeatureFlags/FeatureFlagSettings.h"

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
        // TODO: Tim's Toolkit logger cat
        UE_LOG(LogTemp, Warning, TEXT("Unable to find feature with name \"%s\""), &FeatureName);
        return settings->bDefaultFeatureState;
    }
}
void UFeatureFlagFunctionLibrary::IsFeatureEnabled_SplitExecution(const FName FeatureName, EFeatureState& OutBranch)
{
    OutBranch = IsFeatureEnabled(FeatureName) ? EFeatureState::Enabled : EFeatureState::Disabled;
}
