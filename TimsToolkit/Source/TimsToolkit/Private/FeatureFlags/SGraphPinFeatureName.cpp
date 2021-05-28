#include "FeatureFlags/SGraphPinFeatureName.h"

#include "FeatureFlags/FeatureFlagSettings.h"

void SGraphPinFeatureName::Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj)
{
    RefreshNameList();
    SGraphPinNameList::Construct(SGraphPinNameList::FArguments(), InGraphPinObj, NameList);
}

SGraphPinFeatureName::SGraphPinFeatureName()
{

}

SGraphPinFeatureName::~SGraphPinFeatureName()
{

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
}
