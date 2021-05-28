// Copyright Epic Games, Inc. All Rights Reserved.

#include "TimsToolkitBPLibrary.h"
#include "TimsToolkit.h"

#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/UserInterfaceSettings.h"


UTimsToolkitBPLibrary::UTimsToolkitBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

bool UTimsToolkitBPLibrary::IsEditor()
{
#if WITH_EDITOR
    return true;
#else
    return false;
#endif
}

bool UTimsToolkitBPLibrary::IsShippingBuild()
{
#if UE_BUILD_SHIPPING
    return true;
#else
    return false;
#endif
}

void UTimsToolkitBPLibrary::GetWorldExtent(const AActor* WorldContextObject, const FVector MaximumObjectExtents, const TArray<TSubclassOf<AActor>> IgnoreMaximumExtentsForClasses, FVector& WorldExtent, FVector& WorldCenter)
{
    if (!WorldContextObject)
    {
        UE_LOG(LogTimsToolkit, Warning, TEXT("Invalid world context object!"));
        return;
    }

    // Get actors
    TArray<AActor*> allActors;
    UGameplayStatics::GetAllActorsOfClass(WorldContextObject, TSubclassOf<AActor>(AActor::StaticClass()), allActors);

    FVector origin;
    FVector extent;
    // Filter actors
    for (int i = allActors.Num() - 1; i >= 0; --i)
    {
        AActor* act = allActors[i];
        if (!act)
        {
            continue;
        }

        act->GetActorBounds(true, origin, extent);

        // Check if the class should be filtered
        bool canBeFiltered = true;
        for (size_t ignoredIndex = 0; ignoredIndex < IgnoreMaximumExtentsForClasses.Num(); ++ignoredIndex)
        {
            if (!IgnoreMaximumExtentsForClasses[ignoredIndex])
            {
                continue;
            }

            if (act->GetClass()->IsChildOf(IgnoreMaximumExtentsForClasses[ignoredIndex]))
            {
                canBeFiltered = false;
                break;
            }
        }

        if (!canBeFiltered)
        {
            continue;
        }
        
        // Check if the extents should be filtered
        for (size_t j = 0; j < 3; ++j)
        {
            if (MaximumObjectExtents[j] > 0.0f && extent[j] > MaximumObjectExtents[j])
            {
                allActors.RemoveAt(i);
                break;
            }
        }
    }

    // Get bounds of array
    UGameplayStatics::GetActorArrayBounds(allActors, true, WorldCenter, WorldExtent);
}

void UTimsToolkitBPLibrary::SlateUnitsToPixelsFloat(const float& SlateUnits, float& Pixels)
{
    const float scale = GetViewportScale();
    Pixels = SlateUnits * scale;
}

void UTimsToolkitBPLibrary::SlateUnitsToPixelsVec2D(const FVector2D& SlateUnits, FVector2D& Pixels)
{
    const float scale = GetViewportScale();
    Pixels.X = SlateUnits.X * scale;
    Pixels.Y = SlateUnits.Y * scale;
}

void UTimsToolkitBPLibrary::PixelsToSlateUnitsFloat(const float& Pixels, float& SlateUnits)
{
    const float scale = GetViewportScale();
    if (scale > 0.0f)
    {
        SlateUnits = Pixels / scale;
    }
}

void UTimsToolkitBPLibrary::PixelsToSlateUnitsVec2D(const FVector2D& Pixels, FVector2D& SlateUnits)
{
    const float scale = GetViewportScale();
    if (scale > 0.0f)
    {
        SlateUnits.X = Pixels.X / scale;
        SlateUnits.Y = Pixels.Y / scale;
    }
}

// Based on https://answers.unrealengine.com/questions/120050/current-dpi-scaling.html#answer-216665
float UTimsToolkitBPLibrary::GetViewportScale()
{
    FVector2D viewportSize;
    GEngine->GameViewport->GetViewportSize(viewportSize);
    
    return GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(viewportSize.IntPoint());
}
