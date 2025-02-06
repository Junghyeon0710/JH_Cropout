// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ResourcesTypes/ResourcesTypes.h"
#include "Layer_Game_ActivatableWidget.generated.h"

class UPauseActivatableWidget;
class UEndGameActivatableWidget;
class UCommonActivatableWidgetStack;
class UJHCommonButtonBase;
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

	UPROPERTY(meta=(BindWidget) ,EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	TObjectPtr<UJHCommonButtonBase> BTN_Pause;
	
	UPROPERTY(meta=(BindWidget) ,EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	TObjectPtr<UCommonActivatableWidgetStack> MainStack;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	TSubclassOf<UResourceWidget> ResourceWidgetClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	TSubclassOf<UPauseActivatableWidget> PauseWidgetClass;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	TSubclassOf<UEndGameActivatableWidget> EndGameWidgetClass;

public:

	void AddStackItem(const TSubclassOf<UCommonActivatableWidget>& UCommonActivatableWidgetClass) const;
	void PullCurrentActiveWidget() const;
	void EndGame(bool Win);
};
