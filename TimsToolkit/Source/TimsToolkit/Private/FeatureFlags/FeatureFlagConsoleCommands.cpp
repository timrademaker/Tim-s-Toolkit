#include "FeatureFlags/FeatureFlagConsoleCommands.h"

#if !UE_BUILD_SHIPPING
#include "FeatureFlags/FeatureFlagSettings.h"

#include "HAL/IConsoleManager.h"

static void SetFeatureEnabled(const TArray<FString>& Args)
{
    const int32 numArgs = Args.Num();

    if (numArgs == 0)
    {
        UE_LOG(LogTimsToolkit, Warning, TEXT("No argument provided for console command 'features.SetFeatureEnabled'. Arguments: FeatureName [EnabledState]"));
        return;
    }

    bool newEnabled = true;
    FName featureName = NAME_None;

    if (numArgs >= 1)
    {
        featureName = FName(Args[0]);
    }
    
    if (numArgs >= 2)
    {
        newEnabled = Args[1].ToBool();
    }

    UFeatureFlagSettings* settings = UFeatureFlagSettings::Get();
    
    if(settings)
    {
        bool* featureCurrentlyEnabledPtr = settings->FeatureFlags.Find(featureName);

        if(featureCurrentlyEnabledPtr)
        {
            *featureCurrentlyEnabledPtr = newEnabled;
            UE_LOG(LogTimsToolkit, Log, TEXT("Successfully %s '%s'"), newEnabled ? TEXT("enabled") : TEXT("disabled"), *featureName.ToString());
        }
        else
        {
            UE_LOG(LogTimsToolkit, Warning, TEXT("Unable to find feature with name '%s'"), *featureName.ToString());
        }
    }
}

FAutoConsoleCommand SetFeatureEnabledCmd(
    TEXT("features.SetFeatureEnabled"),
    TEXT("Enable or disable a feature flag. Arguments: FeatureName [EnabledState]"),
    FConsoleCommandWithArgsDelegate::CreateStatic(&SetFeatureEnabled)
);

#endif
