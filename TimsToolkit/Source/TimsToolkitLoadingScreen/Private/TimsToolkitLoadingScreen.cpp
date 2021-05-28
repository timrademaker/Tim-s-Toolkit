#include "TimsToolkitLoadingScreen.h"

#include "TimsToolkitLoadingScreenSettings.h"

#include "Modules/ModuleInterface.h"

#if WITH_EDITOR
#include "ISettingsModule.h"
#endif

#include "Math/UnrealMathUtility.h"
#include "MoviePlayer.h"
#include "Blueprint/UserWidget.h"
#include "Modules/ModuleManager.h"

#include <cassert>


#define LOCTEXT_NAMESPACE "FTimsToolkitLoadingScreenModule"

void FTimsToolkitLoadingScreenModule::StartupModule()
{
#if WITH_EDITOR
    // Register to editor
    ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
    if (SettingsModule)
    {
        SettingsModule->RegisterSettings("Project", "Plugins", "Tim's Toolkit Loading Screen", LOCTEXT("TimsToolkitName", "Tim's Toolkit - Loading Screen"), LOCTEXT("TimsToolkitDescription", "Tim's Toolkit Loading Screen settings"), UTimsToolkitLoadingScreenSettings::Get());
    }
#endif
    
    if (!IsRunningDedicatedServer() && FSlateApplication::IsInitialized())
    {
        if (IsMoviePlayerEnabled() && UTimsToolkitLoadingScreenSettings::Get()->LoadingScreenWidgets.Num() > 0)
        {
            GetMoviePlayer()->OnPrepareLoadingScreen().AddRaw(this, &FTimsToolkitLoadingScreenModule::BeginLoadingScreen);
        }
    }

    FCoreUObjectDelegates::PostLoadMapWithWorld.AddRaw(this, &FTimsToolkitLoadingScreenModule::OnLevelLoaded);
}

void FTimsToolkitLoadingScreenModule::ShutdownModule()
{
#if WITH_EDITOR
    // Unregister from editor
    ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
    if (SettingsModule)
    {
        SettingsModule->UnregisterSettings("Project", "Plugins", "Tim's Toolkit");
    }
#endif

    if (!IsRunningDedicatedServer())
    {
        GetMoviePlayer()->OnPrepareLoadingScreen().RemoveAll(this);
    }
}

void FTimsToolkitLoadingScreenModule::BeginLoadingScreen()
{
    UWorld* world = CurrentWorld;
    if (!IsValid(world))
    {
        // If the world pointer we have is not valid, try to use the global world pointer
        // This should be avoided whenever possible
        if (!GWorld)
        {
            UE_LOG(LogTemp, Error, TEXT("Unable to get world context for the loading screen!"));
            return;
        }

        world = GWorld;
    }

    auto* settings = UTimsToolkitLoadingScreenSettings::Get();
    if (!settings)
    {
        return;
    }

    FLoadingScreenAttributes loadingScreen;
    loadingScreen.bAutoCompleteWhenLoadingCompletes = true;
    
    int ind = FMath::RandRange(0, settings->LoadingScreenWidgets.Num() - 1);

    UUserWidget* widget = CreateWidget(world, settings->LoadingScreenWidgets[ind]);

    loadingScreen.WidgetLoadingScreen = widget->TakeWidget();
    loadingScreen.MinimumLoadingScreenDisplayTime = 1.0f;

    GetMoviePlayer()->SetupLoadingScreen(loadingScreen);
}

void FTimsToolkitLoadingScreenModule::OnLevelLoaded(UWorld* LoadedWorld)
{
    CurrentWorld = LoadedWorld;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTimsToolkitLoadingScreenModule, TimsToolkitLoadingScreen)
