#include "TimsToolkitLoadingScreenSettings.h"

#include "UObject/UObjectGlobals.h"

UTimsToolkitLoadingScreenSettings* UTimsToolkitLoadingScreenSettings::m_EditorSettingsSingleton = nullptr;


UTimsToolkitLoadingScreenSettings* UTimsToolkitLoadingScreenSettings::Get()
{
    if (!m_EditorSettingsSingleton)
    {
        static const TCHAR* settingsContainerName = TEXT("TimsToolkitLoadingScreenSettingsContainer");
        m_EditorSettingsSingleton = FindObject<UTimsToolkitLoadingScreenSettings>(GetTransientPackage(), settingsContainerName);

        if (!m_EditorSettingsSingleton)
        {
            m_EditorSettingsSingleton = NewObject<UTimsToolkitLoadingScreenSettings>(GetTransientPackage(), UTimsToolkitLoadingScreenSettings::StaticClass(), settingsContainerName);
            m_EditorSettingsSingleton->AddToRoot();
        }
    }

    return m_EditorSettingsSingleton;
}
