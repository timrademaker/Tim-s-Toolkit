#pragma once

#include "CoreMinimal.h"
#include "EdGraphUtilities.h"

class SGraphPin;


class FIsFeatureEnabledPinFactory : public FGraphPanelPinFactory
{
	virtual TSharedPtr<class SGraphPin> CreatePin(class UEdGraphPin* InPin) const override;
};
