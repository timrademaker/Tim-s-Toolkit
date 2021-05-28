#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "SGraphPinNameList.h"


class SGraphPinFeatureName : public SGraphPinNameList
{
public:
	SLATE_BEGIN_ARGS(SGraphPinFeatureName) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj);

	SGraphPinFeatureName();
	virtual ~SGraphPinFeatureName();

protected:
	void RefreshNameList();
};

// TODO: Make sure RefreshNameList is called when the feature list is updated
