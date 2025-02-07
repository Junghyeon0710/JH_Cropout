// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "PauseActivatableWidget.generated.h"

class USliderWidget;
class UJHCommonButtonBase;
/**
 * 
 */
UCLASS()
class JH_CROPOUT_API UPauseActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeOnActivated() override;

private:

	UPROPERTY(meta=(BindWidget) ,EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	TObjectPtr<UJHCommonButtonBase> BTN_Resume;

	UPROPERTY(meta=(BindWidget) ,EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	TObjectPtr<UJHCommonButtonBase> BTN_Restart;

	UPROPERTY(meta=(BindWidget) ,EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	TObjectPtr<UJHCommonButtonBase> BTN_MainMenu;

	UPROPERTY(meta=(BindWidget) ,EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	TObjectPtr<USliderWidget> Slider_Music;

	UPROPERTY(meta=(BindWidget) ,EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	TObjectPtr<USliderWidget> Slider_SFX;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	TSoftObjectPtr<UWorld> MainMenuLevel;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	TSoftObjectPtr<UWorld> VillageLevel;
	
};
