#include "TimsToolkitLoadingScreenSettings.h"

#include "UObject/UObjectGlobals.h"

UTimsToolkitLoadingScreenSettings* UTimsToolkitLoadingScreenSettings::LoadingScreenEditorSettingsSingleton = nullptr;


UTimsToolkitLoadingScreenSettings* UTimsToolkitLoadingScreenSettings::Get()
{
    if (!LoadingScreenEditorSettingsSingleton)
    {
        static const TCHAR* settingsContainerName = TEXT("TimsToolkitLoadingScreenSettingsContainer");
        LoadingScreenEditorSettingsSingleton = FindObject<UTimsToolkitLoadingScreenSettings>(GetTransientPackage(), settingsContainerName);

        if (!LoadingScreenEditorSettingsSingleton)
        {
            LoadingScreenEditorSettingsSingleton = NewObject<UTimsToolkitLoadingScreenSettings>(GetTransientPackage(), UTimsToolkitLoadingScreenSettings::StaticClass(), settingsContainerName);
            LoadingScreenEditorSettingsSingleton->AddToRoot();
        }
    }

    return LoadingScreenEditorSettingsSingleton;
}
