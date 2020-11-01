// Copyright Epic Games, Inc. All Rights Reserved.

#include "TimsToolkitBPLibrary.h"
#include "TimsToolkit.h"

#include "Kismet/GameplayStatics.h"
#include "Engine.h"

#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"


UTimsToolkitBPLibrary::UTimsToolkitBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

bool UTimsToolkitBPLibrary::IsEditor()
{
#if WITH_EDITOR
    return true;
#else
    return false;
#endif
}

void UTimsToolkitBPLibrary::GetWorldExtent(const AActor* WorldContextObject, const FVector MaximumObjectExtents, const TArray<TSubclassOf<AActor>> IgnoreMaximumExtentsForClasses, FVector& WorldExtent, FVector& WorldCenter)
{
    if (!WorldContextObject)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid world context object!"));
        return;
    }

    // Get actors
    TArray<AActor*> allActors;
    UGameplayStatics::GetAllActorsOfClass(WorldContextObject, TSubclassOf<AActor>(AActor::StaticClass()), allActors);

    FVector origin;
    FVector extent;
    // Filter actors
    for (int i = allActors.Num() - 1; i >= 0; --i)
    {
        AActor* act = allActors[i];
        if (!act)
        {
            continue;
        }

        act->GetActorBounds(true, origin, extent);

        // Check if the class should be filtered
        bool canBeFiltered = true;
        for (size_t ignoredIndex = 0; ignoredIndex < IgnoreMaximumExtentsForClasses.Num(); ++ignoredIndex)
        {
            if (!IgnoreMaximumExtentsForClasses[ignoredIndex])
            {
                continue;
            }

            if (act->GetClass()->IsChildOf(IgnoreMaximumExtentsForClasses[ignoredIndex]))
            {
                canBeFiltered = false;
                break;
            }
        }

        if (!canBeFiltered)
        {
            continue;
        }
        
        // Check if the extents should be filtered
        for (size_t j = 0; j < 3; ++j)
        {
            if (MaximumObjectExtents[j] > 0.0f && extent[j] > MaximumObjectExtents[j])
            {
                allActors.RemoveAt(i);
                break;
            }
        }
    }

    // Get bounds of array
    UGameplayStatics::GetActorArrayBounds(allActors, true, WorldCenter, WorldExtent);
}

void UTimsToolkitBPLibrary::SendToDiscordWebhook(const FString& WebhookUrl, const TArray<FString> Attachments, const TArray<FDiscordEmbed> Embeds, const FString MessageContent, const FString Nickname, const FString AvatarUrl)
{
    // Make json string
    FString messageJson = "{";

    // Content
    if (MessageContent.Len() > 0)
    {
        messageJson += "\"content\": \"" + MessageContent.ReplaceCharWithEscapedChar() + "\",";
    }

    // Username
    if (Nickname.Len() > 0)
    {
        messageJson += "\"username\": \"" + Nickname.ReplaceCharWithEscapedChar() + "\",";
    }

    // Avatar
    if (AvatarUrl.Len() > 0)
    {
        messageJson += "\"avatar_url\": \"" + AvatarUrl.ReplaceCharWithEscapedChar() + "\",";
    }

    // Embeds
    if (Embeds.Num() > 0)
    {
        messageJson += "\"embeds\": [";

        FString embedJson;
        for (int i = 0; i < Embeds.Num(); ++i)
        {
            UTimsToolkitBPLibrary::DiscordWebhookEmbedToJson(Embeds[i], embedJson);

            messageJson += embedJson + ",";
        }

        messageJson.RemoveFromEnd(",");
        messageJson += "]";
    }
    else
    {
        messageJson.RemoveFromEnd(",");
    }

    messageJson += "}";

    // Send
    FHttpModule& http = FHttpModule::Get();
    TSharedRef <IHttpRequest> request = http.CreateRequest();
    request->SetURL(WebhookUrl);
    request->SetVerb("POST");
    request->SetHeader("Content-Type", "application/json");
    request->SetContentAsString(messageJson);

    if (!request->ProcessRequest())
    {
        // Something went wrong while starting request processing
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Unable to start web request");
    }

    request->OnProcessRequestComplete().BindLambda([](FHttpRequestPtr /*Request*/, FHttpResponsePtr Response, bool /*bConnectedSuccessfully*/)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, Response->GetContentAsString());
            UE_LOG(LogTemp, Warning, TEXT("%s"), *Response->GetContentAsString());
        });
}

void UTimsToolkitBPLibrary::DiscordWebhookEmbedToJson(const FDiscordEmbed& Embed, FString& Json)
{
    Json = "";

    // Title
    Json = "{\"title\": \"" + Embed.Title.ReplaceCharWithEscapedChar() + "\",";

    // Content
    if (Embed.Content.Len() > 0)
    {
        Json += "\"content\": \"" + Embed.Content.ReplaceCharWithEscapedChar() + "\",";
    }
    
    // Color
    FString embedColor;
    UTimsToolkitBPLibrary::ColorToInteger(Embed.Color, embedColor);
    Json += "\"color\": " + embedColor + ",";

    // Footer
    if (Embed.Footer.Len() > 0)
    {
        Json += "\"footer\": {\"text\": \"" + Embed.Footer.ReplaceCharWithEscapedChar() + "\"},";
    }

    // Fields
    if (Embed.Fields.Num() > 0)
    {
        if (Embed.Fields[0].Name.Len() > 0)
        {
            Json += "\"fields\": [";

            FString fieldJson;
            for (int i = 0; i < Embed.Fields.Num(); ++i)
            {
                UTimsToolkitBPLibrary::DiscordWebhookFieldToJson(Embed.Fields[i], fieldJson);
                Json += fieldJson + ",";
            }

            Json.RemoveFromEnd(",");

            Json += "]";
        }
        else
        {
            Json.RemoveFromEnd(",");
        }
    }
    else
    {
        Json.RemoveFromEnd(",");
    }

    Json += "}";
}

void UTimsToolkitBPLibrary::DiscordWebhookFieldToJson(const FDiscordEmbedField& Field, FString& Json)
{
    Json = "";

    if (Field.Name.Len() > 0)
    {
        // name
        Json = "{\"name\": \"" + Field.Name.ReplaceCharWithEscapedChar() + "\",";
        // value
        if (Field.Value.Len() > 0)
        {
            Json += "\"value\": \"" + Field.Value.ReplaceCharWithEscapedChar() + "\"";
        }
        else
        {
            Json.RemoveFromEnd(",");
        }

        Json += "}";
    }
}

void UTimsToolkitBPLibrary::ColorToInteger(const FColor& Color, FString& Integer)
{
    // Remove alpha from the color
    FColor col2 = Color;
    col2.A = 0;
    
    // Create integer string
    Integer = FString::FromInt(col2.ToPackedARGB());
}
