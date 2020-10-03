#include "TimsToolkitLoadingScreen.h"

#include "TimsToolkitLoadingScreenSettings.h"

#include "Modules/ModuleInterface.h"
#include "ISettingsModule.h"
#include "PropertyEditorModule.h"
#include "Math/UnrealMathUtility.h"
#include "MoviePlayer.h"
#include "Blueprint/UserWidget.h"
#include "Modules/ModuleManager.h"

#include <cassert>


#define LOCTEXT_NAMESPACE "FTimsToolkitLoadingScreenModule"

void FTimsToolkitLoadingScreenModule::StartupModule()
{
    // Register to editor
    ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
    if (SettingsModule)
    {
        SettingsModule->RegisterSettings("Project", "Plugins", "Tim's Toolkit", LOCTEXT("TimsToolkitName", "Tim's Toolkit"), LOCTEXT("TimsToolkitDescription", "Tim's Toolkit settings"), UTimsToolkitLoadingScreenSettings::Get());
    }
    
    if (!IsRunningDedicatedServer() && FSlateApplication::IsInitialized())
    {
        if (IsMoviePlayerEnabled() && UTimsToolkitLoadingScreenSettings::Get()->m_LoadingScreenWidgets.Num() > 0)
        {
            GetMoviePlayer()->OnPrepareLoadingScreen().AddRaw(this, &FTimsToolkitLoadingScreenModule::BeginLoadingScreen);
        }
    }
}

void FTimsToolkitLoadingScreenModule::ShutdownModule()
{
    // Unregister from editor
    ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
    if (SettingsModule)
    {
        SettingsModule->UnregisterSettings("Project", "Plugins", "Tim's Toolkit");
    }

    if (!IsRunningDedicatedServer())
    {
        GetMoviePlayer()->OnPrepareLoadingScreen().RemoveAll(this);
    }
}

void FTimsToolkitLoadingScreenModule::BeginLoadingScreen()
{
    // GWorld is used to create an instance of the loading screen widget.
    // TODO: Find a better way to create the widget
    if (!GWorld)
    {
        return;
    }

    auto* settings = UTimsToolkitLoadingScreenSettings::Get();
    if (!settings)
    {
        return;
    }

    FLoadingScreenAttributes loadingScreen;
    loadingScreen.bAutoCompleteWhenLoadingCompletes = true;
    
    int ind = FMath::RandRange(0, settings->m_LoadingScreenWidgets.Num() - 1);

    UWorld* world = GWorld;
    UUserWidget* widget = CreateWidget(world, settings->m_LoadingScreenWidgets[ind]);

    loadingScreen.WidgetLoadingScreen = widget->TakeWidget();
    loadingScreen.MinimumLoadingScreenDisplayTime = 1.0f;

    GetMoviePlayer()->SetupLoadingScreen(loadingScreen);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTimsToolkitLoadingScreenModule, TimsToolkitLoadingScreen)
