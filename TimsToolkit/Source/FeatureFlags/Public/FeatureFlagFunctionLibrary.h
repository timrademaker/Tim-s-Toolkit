#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "FeatureFlagFunctionLibrary.generated.h"

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
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Category = "Tim's Toolkit|Feature Flags", AutoCreateRefTerm = "FeatureName", BlueprintInternalUseOnly = "true"))
    static bool IsFeatureEnabled(const FName& FeatureName);
};
