#include "DiscordWebhookBPLibrary.h"
#include "TimsToolkit.h"

#include "Engine.h"

#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"

UDiscordWebhookBPLibrary::UDiscordWebhookBPLibrary(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void UDiscordWebhookBPLibrary::SendMessageToDiscordWebhook(const FString& WebhookUrl, const TArray<FDiscordEmbed> Embeds, const TArray<FString> AttachmentPaths, const FString& MessageContent, const FString& Nickname, const FString& AvatarUrl)
{
    FHttpModule& http = FHttpModule::Get();
    if (!http.IsHttpEnabled())
    {
        UE_LOG(LogTemp, Warning, TEXT("HTTP is disabled!"));
    }
    
    // Make json string
    FString messageJson;
    UDiscordWebhookBPLibrary::ConstructMessageJson(MessageContent, Embeds, Nickname, AvatarUrl, messageJson);

    // Send
    TSharedRef <IHttpRequest> request = http.CreateRequest();
    request->SetURL(WebhookUrl);
    request->SetVerb("POST");

    if (AttachmentPaths.Num() == 0)
    {
        request->SetHeader("Content-Type", "application/json");
        request->SetContentAsString(messageJson);
    }
    else
    {
        FString contentTypeHeader;
        TArray<uint8> content;
        UDiscordWebhookBPLibrary::ConstructMultipartContent(AttachmentPaths, messageJson, contentTypeHeader, content);

        request->SetHeader("Content-Type", contentTypeHeader);
        request->SetContent(content);
    }

    if (!request->ProcessRequest())
    {
        UE_LOG(LogTemp, Warning, TEXT("Unable to start web request"));
    }

    request->OnProcessRequestComplete().BindLambda([](FHttpRequestPtr /*Request*/, FHttpResponsePtr Response, bool ConnectedSuccessfully)
        {
            if (!ConnectedSuccessfully)
            {
                UE_LOG(LogTemp, Warning, TEXT("Issue while sending a message to a Discord webhook. Response: %s"), *Response->GetContentAsString());
            }
        }
    );
}

void UDiscordWebhookBPLibrary::ConstructMessageJson(const FString& MessageContent, const TArray<FDiscordEmbed>& Embeds, const FString& Nickname, const FString& AvatarUrl, FString& MessageJson)
{
    MessageJson = "{";

    // Content
    if (MessageContent.Len() > 0)
    {
        MessageJson += "\"content\": \"" + MessageContent.ReplaceCharWithEscapedChar() + "\",";
    }

    // Username
    if (Nickname.Len() > 0)
    {
        MessageJson += "\"username\": \"" + Nickname.ReplaceCharWithEscapedChar() + "\",";
    }

    // Avatar
    if (AvatarUrl.Len() > 0)
    {
        MessageJson += "\"avatar_url\": \"" + AvatarUrl.ReplaceCharWithEscapedChar() + "\",";
    }

    // Embeds
    if (Embeds.Num() > 0)
    {
        MessageJson += "\"embeds\": [";

        FString embedJson;
        for (int i = 0; i < Embeds.Num(); ++i)
        {
            UDiscordWebhookBPLibrary::ConstructEmbedJson(Embeds[i], embedJson);

            MessageJson += embedJson + ",";
        }

        MessageJson.RemoveFromEnd(",");
        MessageJson += "]";
    }
    else
    {
        MessageJson.RemoveFromEnd(",");
    }

    MessageJson += "}";
}

void UDiscordWebhookBPLibrary::ConstructMultipartContent(const TArray<FString>& AttachmentPaths, const FString& MessageJson, FString& ContentTypeHeader, TArray<uint8>& MultipartContent)
{
    const FString boundary = "-----BOUNDARY-----";
    const FString midBoundary = "\r\n--" + boundary + "\r\n";
    const FString endBoundary = "\r\n--" + boundary + "--\r\n";
    
    ContentTypeHeader = "multipart/form-data; boundary=" + boundary;

    for (int i = 0; i < AttachmentPaths.Num(); ++i)
    {
        if (!FPaths::FileExists(AttachmentPaths[i]))
        {
            continue;
        }

        MultipartContent.Append((uint8*)TCHAR_TO_UTF8(*midBoundary), midBoundary.Len());

        const FString fileName = FPaths::GetCleanFilename(AttachmentPaths[i]);

        const FString attachmentString = "Content-Disposition: form-data; name=\"file" + FString::FromInt(i) + "\"; filename=\"" + fileName + "\"\r\nContent-Type: application/octet-stream\r\n\r\n";

        TArray<uint8> rawData;
        FFileHelper::LoadFileToArray(rawData, *AttachmentPaths[i]);

        MultipartContent.Append((uint8*)TCHAR_TO_UTF8(*attachmentString), attachmentString.Len());
        MultipartContent.Append(rawData);
    }

    if (MessageJson.Len() > 0)
    {
        const FString payload = midBoundary + "Content-Disposition: form-data; name=\"payload_json\"\r\n\r\n" + MessageJson + "\r\n";
        MultipartContent.Append((uint8*)TCHAR_TO_UTF8(*payload), payload.Len());
    }

    MultipartContent.Append((uint8*)TCHAR_TO_UTF8(*endBoundary), endBoundary.Len());
}

void UDiscordWebhookBPLibrary::ConstructEmbedJson(const FDiscordEmbed& Embed, FString& EmbedJson)
{
    // Title
    EmbedJson = "{\"title\": \"" + Embed.Title.ReplaceCharWithEscapedChar() + "\",";

    // Content
    if (Embed.Content.Len() > 0)
    {
        EmbedJson += "\"content\": \"" + Embed.Content.ReplaceCharWithEscapedChar() + "\",";
    }

    // Color
    FString embedColor;
    UDiscordWebhookBPLibrary::ColorToInteger(Embed.Color, embedColor);
    EmbedJson += "\"color\": " + embedColor + ",";

    // Footer
    if (Embed.Footer.Len() > 0)
    {
        EmbedJson += "\"footer\": {\"text\": \"" + Embed.Footer.ReplaceCharWithEscapedChar() + "\"},";
    }

    // Fields
    if (Embed.Fields.Num() > 0)
    {
        if (Embed.Fields[0].Name.Len() > 0)
        {
            EmbedJson += "\"fields\": [";

            FString fieldJson;
            for (int i = 0; i < Embed.Fields.Num(); ++i)
            {
                UDiscordWebhookBPLibrary::ConstructFieldJson(Embed.Fields[i], fieldJson);
                EmbedJson += fieldJson + ",";
            }

            EmbedJson.RemoveFromEnd(",");

            EmbedJson += "]";
        }
        else
        {
            EmbedJson.RemoveFromEnd(",");
        }
    }
    else
    {
        EmbedJson.RemoveFromEnd(",");
    }

    EmbedJson += "}";
}

void UDiscordWebhookBPLibrary::ConstructFieldJson(const FDiscordEmbedField& Field, FString& FieldJson)
{
    FieldJson = "";

    if (Field.Name.Len() > 0)
    {
        // name
        FieldJson = "{\"name\": \"" + Field.Name.ReplaceCharWithEscapedChar() + "\",";
        // value
        if (Field.Value.Len() > 0)
        {
            FieldJson += "\"value\": \"" + Field.Value.ReplaceCharWithEscapedChar() + "\"";
        }
        else
        {
            FieldJson.RemoveFromEnd(",");
        }

        FieldJson += "}";
    }
}

void UDiscordWebhookBPLibrary::ColorToInteger(const FColor& Color, FString& Integer)
{
    // Remove alpha from the color
    FColor col2 = Color;
    col2.A = 0;

    // Create integer string
    Integer = FString::FromInt(col2.ToPackedARGB());
}
