#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "DiscordEmbedStructs.h"

#include "TimsToolkitBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
UCLASS()
class UTimsToolkitBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "Is Editor", Tooltip = "Returns true if the function is executed from the editor", Category = "Tim's Toolkit"))
	static bool IsEditor();

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "Get World Extent", Tooltip = "Gets the world dimensions", Category = "Tim's Toolkit", AutoCreateRefTerm = "IgnoreMaximumExtentsForClasses"))
	static void GetWorldExtent(const AActor* WorldContextObject, const FVector MaximumObjectExtents, const TArray<TSubclassOf<AActor>> IgnoreMaximumExtentsForClasses, FVector& WorldExtent, FVector& WorldCenter);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Send Message to Discord Webhook", Tooltip = "Send a message to a Discord webhook", Category = "Tim's Toolkit", AutoCreateRefTerm = "Embeds,AttachmentPaths,MessageContent,Nickname,AvatarUrl"))
	static void SendMessageToDiscordWebhook(const FString& WebhookUrl, const TArray<FDiscordEmbed> Embeds, const TArray<FString> AttachmentPaths, const FString MessageContent = "", const FString Nickname = "", const FString AvatarUrl = "");

private:
	static void DiscordWebhookEmbedToJson(const FDiscordEmbed& Embed, FString& Json);
	static void DiscordWebhookFieldToJson(const FDiscordEmbedField& Field, FString& Json);

	static void ColorToInteger(const FColor& Color, FString& Integer);
};
