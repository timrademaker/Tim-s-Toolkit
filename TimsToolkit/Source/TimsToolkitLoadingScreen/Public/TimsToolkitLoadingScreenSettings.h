#pragma once

#include "Engine/DeveloperSettings.h"
#include "Components/Widget.h"

#include "TimsToolkitLoadingScreenSettings.generated.h"


UCLASS(Config=Game, defaultconfig, meta = (DisplayName = "Loading Screen"))
class TIMSTOOLKITLOADINGSCREEN_API UTimsToolkitLoadingScreenSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    static UTimsToolkitLoadingScreenSettings* Get();


    // Array of widgets
    UPROPERTY(Config, EditAnywhere, Category = LoadingScreen, Meta=(ToolTip="One of these widgets is randomly selected and shown when the game loads a new level", DisplayName="Loading Screen Widgets"))
    TArray<TSubclassOf<UUserWidget>> m_LoadingScreenWidgets;

private:
    static UTimsToolkitLoadingScreenSettings* m_EditorSettingsSingleton;

};