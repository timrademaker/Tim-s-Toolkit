#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "TimsToolkitBPLibrary.generated.h"

UCLASS()
class UTimsToolkitBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "Is Editor", Tooltip = "Returns true if the function is executed from the editor", Category = "Tim's Toolkit"))
	static bool IsEditor();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "Get World Extent", Tooltip = "Gets the world dimensions", Category = "Tim's Toolkit", AutoCreateRefTerm = "IgnoreMaximumExtentsForClasses"))
	static void GetWorldExtent(const AActor* WorldContextObject, const FVector MaximumObjectExtents, const TArray<TSubclassOf<AActor>> IgnoreMaximumExtentsForClasses, FVector& WorldExtent, FVector& WorldCenter);


	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "Slate Units to Pixels (float)", Tooltip = "Convert slate units to pixels", Category = "Tim's Toolkit"))
	static void SlateUnitsToPixelsFloat(const float& SlateUnits, float& Pixels);
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "Slate Units to Pixels (Vector2D)", Tooltip = "Convert slate units to pixels", Category = "Tim's Toolkit"))
	static void SlateUnitsToPixelsVec2D(const FVector2D& SlateUnits, FVector2D& Pixels);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "Pixels to Slate Units (float)", Tooltip = "Convert pixels to slate units", Category = "Tim's Toolkit"))
	static void PixelsToSlateUnitsFloat(const float& Pixels, float& SlateUnits);
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "Pixels to Slate Units (Vector2D)", Tooltip = "Convert pixels to slate units", Category = "Tim's Toolkit"))
	static void PixelsToSlateUnitsVec2D(const FVector2D& Pixels, FVector2D& SlateUnits);

protected:
	static float GetViewportScale();
};
