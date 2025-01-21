// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ResourcesTypes/ResourcesTypes.h"
#include "Layer_Game_ActivatableWidget.generated.h"

class UTextBlock;
class UResourceWidget;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class JH_CROPOUT_API ULayer_Game_ActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

private:

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	EResourceType Resources;
	
	UPROPERTY(meta=(BindWidget) ,EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	TObjectPtr<UVerticalBox> ResourceContainer;

	UPROPERTY(meta=(BindWidget) ,EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	TObjectPtr<UTextBlock> VillagerCounter;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	TSubclassOf<UResourceWidget> ResourceWidgetClass;
};
