#pragma once

#include "DiscordEmbedStructs.generated.h"


USTRUCT(BlueprintType)
struct FDiscordEmbedField
{
    GENERATED_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString Name = "";

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString Value = "";
        
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        bool Inline = false;
};


USTRUCT(BlueprintType)
struct FDiscordEmbed
{
    GENERATED_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString Title = "";

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString Content = "";
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (HideAlphaChannel))
        FLinearColor Color;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TArray<FDiscordEmbedField> Fields;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString Footer = "";
};