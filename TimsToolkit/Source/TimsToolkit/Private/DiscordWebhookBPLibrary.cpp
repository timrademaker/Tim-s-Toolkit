#include "DiscordWebhookBPLibrary.h"
#include "TimsToolkit.h"

#include "Engine.h"

#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"

static constexpr int32 MAX_EMBEDS = 10;
static constexpr int32 MAX_ATTACHMENTS = 10;

UDiscordWebhookBPLibrary::UDiscordWebhookBPLibrary(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void UDiscordWebhookBPLibrary::SendMessageToDiscordWebhook(const FString& WebhookUrl, const TArray<FDiscordEmbed> Embeds, const TArray<FString> AttachmentPaths, const FString& MessageContent, const FString& Nickname, const FString& AvatarUrl)
{
    FHttpModule& http = FHttpModule::Get();
    if (!http.IsHttpEnabled())
    {
        UE_LOG(LogTimsToolkit, Warning, TEXT("Unable to send message to Discord webhook as HTTP is disabled!"));
        return;
    }

    if (Embeds.Num() == 0 && AttachmentPaths.Num() == 0 && MessageContent.IsEmpty())
    {
        UE_LOG(LogTimsToolkit, Warning, TEXT("Can't send an empty message!"));
        return;
    }

    // Make json string
    FString messageJson;
    UDiscordWebhookBPLibrary::ConstructMessageJson(MessageContent, Embeds, Nickname, AvatarUrl, messageJson);

    // Un-escape single quotation mark, because having it escaped causes issues for some reason
    messageJson.ReplaceInline(TEXT("\\\'"), TEXT("\'"));

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

    request->OnProcessRequestComplete().BindLambda([](FHttpRequestPtr /*Request*/, FHttpResponsePtr Response, bool ConnectedSuccessfully)
        {
            if (!ConnectedSuccessfully || Response->GetResponseCode() >= 400)
            {
                UE_LOG(LogTimsToolkit, Warning, TEXT("Issue while sending a message to a Discord webhook. Response: %s"), *Response->GetContentAsString());
                UE_LOG(LogTimsToolkit, Warning, TEXT("Status code: %s"), *FString::FromInt(Response->GetResponseCode()));
            }
        }
    );

    if (!request->ProcessRequest())
    {
        UE_LOG(LogTimsToolkit, Warning, TEXT("Unable to start web request for Discord webhook."));
    }
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
        if (Embeds.Num() > MAX_EMBEDS)
        {
            UE_LOG(LogTimsToolkit, Warning, TEXT("Trying to add too many embeds to a message. The maximum number of embeds is %s."), *FString::FromInt(MAX_EMBEDS));
        }

        MessageJson += "\"embeds\": [";

        FString embedJson;
        for (int i = 0; i < FMath::Min(Embeds.Num(), MAX_EMBEDS); ++i)
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
    if (AttachmentPaths.Num() > MAX_ATTACHMENTS)
    {
        UE_LOG(LogTimsToolkit, Warning, TEXT("Trying to add too many files to a message. The maximum number of files is %s."), *FString::FromInt(MAX_ATTACHMENTS));
    }

    const FString boundary = "-----BOUNDARY-----";
    const FString midBoundary = "\r\n--" + boundary + "\r\n";
    const FString endBoundary = "\r\n--" + boundary + "--\r\n";
    
    ContentTypeHeader = "multipart/form-data; boundary=" + boundary;

    uint8 addedAttachments = 0;

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

        ++addedAttachments;
        if (addedAttachments == MAX_ATTACHMENTS)
        {
            break;
        }
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
            FieldJson += "\"value\": \"" + Field.Value.ReplaceCharWithEscapedChar() + "\",";
        }

        // inline
        if (Field.Inline)
        {
            FieldJson += "\"inline\": true,";
        }
        
        FieldJson.RemoveFromEnd(",");

        FieldJson += "}";
    }
}

void UDiscordWebhookBPLibrary::ColorToInteger(const FLinearColor& Color, FString& Integer)
{
    // Remove alpha from the color
    FColor col2 = Color.ToFColor(true);
    col2.A = 0;

    // Create integer string
    Integer = FString::FromInt(col2.ToPackedARGB());
}
