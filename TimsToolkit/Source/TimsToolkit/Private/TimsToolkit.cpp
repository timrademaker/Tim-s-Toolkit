// Fill out your copyright notice in the Description page of Project Settings.

#include "TimsToolkit.h"

#define LOCTEXT_NAMESPACE "FTimsToolkitModule"

void FTimsToolkitModule::StartupModule()
{
	
}

void FTimsToolkitModule::ShutdownModule()
{
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTimsToolkitModule, TimsToolkit)
DEFINE_LOG_CATEGORY(LogTimsToolkit);
