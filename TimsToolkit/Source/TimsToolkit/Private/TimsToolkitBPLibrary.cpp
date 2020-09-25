// Copyright Epic Games, Inc. All Rights Reserved.

#include "TimsToolkitBPLibrary.h"
#include "TimsToolkit.h"

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

