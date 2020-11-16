#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "DiscordEmbedStructs.h"

#include "DiscordWebhookBPLibrary.generated.h"


UCLASS()
class UDiscordWebhookBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Send Message to Discord Webhook", Tooltip = "Send a message to a Discord webhook", Category = "Tim's Toolkit", AutoCreateRefTerm = "Embeds,AttachmentPaths,MessageContent,Nickname,AvatarUrl"))
		static void SendMessageToDiscordWebhook(const FString& WebhookUrl, const TArray<FDiscordEmbed> Embeds, const TArray<FString> AttachmentPaths, const FString& MessageContent, const FString& Nickname, const FString& AvatarUrl);

private:
	static void ConstructMessageJson(const FString& MessageContent, const TArray<FDiscordEmbed>& Embeds, const FString& Nickname, const FString& AvatarUrl, FString& MessageJson);
	static void ConstructMultipartContent(const TArray<FString>& AttachmentPaths, const FString& MessageJson, FString& ContentTypeHeader, TArray<uint8>& MultipartContent);
	static void ConstructEmbedJson(const FDiscordEmbed& Embed, FString& EmbedJson);
	static void ConstructFieldJson(const FDiscordEmbedField& Field, FString& FieldJson);

	static void ColorToInteger(const FLinearColor& Color, FString& Integer);
};
