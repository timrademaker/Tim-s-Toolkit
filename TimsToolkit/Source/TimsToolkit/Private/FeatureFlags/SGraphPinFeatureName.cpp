#include "FeatureFlags/SGraphPinFeatureName.h"

#include "FeatureFlags/FeatureFlagSettings.h"


void SGraphPinFeatureName::Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj)
{
    RefreshNameList();
    SGraphPinNameList::Construct(SGraphPinNameList::FArguments(), InGraphPinObj, NameList);

    OnFeatureFlagMapChangedDelegateHandle = UFeatureFlagSettings::Get()->OnFeatureFlagMapChanged().AddLambda([this]()
        {
            RefreshNameList();
        }
    );
}

SGraphPinFeatureName::SGraphPinFeatureName()
{

}

SGraphPinFeatureName::~SGraphPinFeatureName()
{
    UFeatureFlagSettings::Get()->OnFeatureFlagMapChanged().Remove(OnFeatureFlagMapChangedDelegateHandle);
}

void SGraphPinFeatureName::RefreshNameList()
{
    NameList.Empty();

    UFeatureFlagSettings* settings = UFeatureFlagSettings::Get();
    
    if(settings)
    {
        for(auto it = settings->FeatureFlags.CreateConstIterator(); it; ++it)
        {
            TSharedPtr<FName> featureName = MakeShareable(new FName(it->Key));
            NameList.Add(featureName);
        }
    }

    // Try to restore previous selection
    if (ComboBox)
    {
        TSharedPtr<FName> selectedName = ComboBox->GetSelectedItem();

        TSharedPtr<FName>* foundName = NameList.FindByPredicate([selectedName](TSharedPtr<FName> Name)
            {
                return Name->IsEqual(*selectedName);
            }
        );

        if (foundName)
        {
            ComboBox->SetSelectedItem(*foundName);
        }
    }
}
