// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResourcesTypes/ResourcesTypes.h"
#include "CostWidget.generated.h"

class UCommonTextBlock;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class JH_CROPOUT_API UCostWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	TObjectPtr<UImage> Image_17;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	TObjectPtr<UCommonTextBlock> C_Cost;

	UPROPERTY(BlueprintReadOnly)
	int32 Cost;

	UPROPERTY(BlueprintReadOnly)
	EResourceType Resource;

protected:
	virtual void NativePreConstruct() override;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> FoodTexture;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> WoodTexture;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> StoneTexture;
};
