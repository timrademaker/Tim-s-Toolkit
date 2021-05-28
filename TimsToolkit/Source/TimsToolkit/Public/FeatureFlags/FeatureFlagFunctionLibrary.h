#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "FeatureFlagFunctionLibrary.generated.h"

UENUM(BlueprintType)
enum class EFeatureState : uint8
{
    Enabled,
    Disabled
};


UCLASS()
class UFeatureFlagFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:
    /**
     * Check if a feature flag is enabled
     * @param FeatureName The name of the feature
     * @return True if the feature is enabled
     */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "Tim's Toolkit|Feature Flags"))
    static bool IsFeatureEnabled(const FName& FeatureName);
    
    /**
     * Check if a feature flag is enabled
     * @param FeatureName The name of the feature
     */
    UFUNCTION(BlueprintCallable, meta = (Category = "Tim's Toolkit|Feature Flags", DisplayName = "Is Feature Enabled", ExpandEnumAsExecs = "OutBranch"))
    static void IsFeatureEnabled_SplitExecution(const FName FeatureName, EFeatureState& OutBranch);
};
