#include "TimsToolkitLoadingScreen.h"

#include "TimsToolkitLoadingScreenSettings.h"

#include "Modules/ModuleInterface.h"
#include "ISettingsModule.h"
#include "PropertyEditorModule.h"
#include "Modules/ModuleManager.h"


#define LOCTEXT_NAMESPACE "FTimsToolkitLoadingScreenModule"

void FTimsToolkitLoadingScreenModule::StartupModule()
{
    // Register to editor
    ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
    if (SettingsModule)
    {
        //SettingsModule->RegisterSettings("Project", "Plugins", "Tim's Toolkit", LOCTEXT("LoadingScreenSettingName", "Loading Screen"), LOCTEXT("LoadingScreenSettingDescription", "Loading screen settings"), GetMutableDefault<UTimsToolkitProjectSettings>());
        SettingsModule->RegisterSettings("Project", "Plugins", "Tim's Toolkit", LOCTEXT("TimsToolkitName", "Tim's Toolkit"), LOCTEXT("TimsToolkitDescription", "Tim's Toolkit settings"), UTimsToolkitLoadingScreenSettings::Get());
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
}
#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTimsToolkitLoadingScreenModule, TimsToolkitLoadingScreen)
