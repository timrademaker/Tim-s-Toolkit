#include "IsFeatureEnabledPinFactory.h"

#include "K2Node_IsFeatureEnabled.h"
#include "SGraphPinFeatureName.h"

#include "EdGraphSchema_K2.h"
#include "SGraphPin.h"

TSharedPtr<class SGraphPin> FIsFeatureEnabledPinFactory::CreatePin(class UEdGraphPin* InPin) const
{
    if (InPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Name)
    {
        UObject* outer = InPin->GetOuter();

        const UEdGraphPin* featureNamePin = nullptr;

        if(outer->IsA(UK2Node_IsFeatureEnabled::StaticClass()))
        {
            const UK2Node_IsFeatureEnabled* isFeatureEnabledNode = CastChecked<UK2Node_IsFeatureEnabled>(outer);
            featureNamePin = isFeatureEnabledNode->GetFeatureNamePin();
        }

        if(featureNamePin)
        {
            if(featureNamePin->DefaultObject == nullptr && featureNamePin->LinkedTo.Num() == 0)
            {
                return SNew(SGraphPinFeatureName, InPin);
            }
        }
    }

    return nullptr;
}
