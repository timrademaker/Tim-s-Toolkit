// Copyright Epic Games, Inc. All Rights Reserved.

#include "TimsToolkitBPLibrary.h"
#include "TimsToolkit.h"

#include "Kismet/GameplayStatics.h"


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

void UTimsToolkitBPLibrary::GetWorldExtent(const AActor* WorldContextObject, const FVector MaximumObjectExtents, const TArray<TSubclassOf<AActor>> IgnoreMaximumExtentsForClasses, FVector& WorldExtent, FVector& WorldCenter)
{
    if (!WorldContextObject)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid world context object!"));
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