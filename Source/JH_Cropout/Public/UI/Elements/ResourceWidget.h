// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResourcesTypes/ResourcesTypes.h"
#include "ResourceWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class JH_CROPOUT_API UResourceWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EResourceType ResourceType;

protected:
	virtual void NativePreConstruct() override;

	UPROPERTY(meta=(BindWidget),EditDefaultsOnly,BlueprintReadWrite)
	TObjectPtr<UImage> Image_24;

	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> NoneTexture;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> FoodTexture;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> WoodTexture;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> StoneTexture;
};
