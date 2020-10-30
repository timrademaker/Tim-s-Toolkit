#pragma once

#include "DiscordEmbedStructs.generated.h"


USTRUCT(BlueprintType)
struct FDiscordEmbedField
{
    GENERATED_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString FieldTitle = "";

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString FieldContent = "";
};


USTRUCT(BlueprintType)
struct FDiscordEmbed
{
    GENERATED_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString EmbedTitle = "";

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString EmbedContent = "";
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString EmbedColor = "";
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        TArray<FDiscordEmbedField> EmbedFields;
};