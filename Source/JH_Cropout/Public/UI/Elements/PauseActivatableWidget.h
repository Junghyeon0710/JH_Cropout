// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "PauseActivatableWidget.generated.h"

class UJHCommonButtonBase;
/**
 * 
 */
UCLASS()
class JH_CROPOUT_API UPauseActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;

private:

	UPROPERTY(meta=(BindWidget) ,EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	TObjectPtr<UJHCommonButtonBase> BTN_Resume;

	UPROPERTY(meta=(BindWidget) ,EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	TObjectPtr<UJHCommonButtonBase> BTN_Restart;

	UPROPERTY(meta=(BindWidget) ,EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	TObjectPtr<UJHCommonButtonBase> BTN_MainMenu;

	
};
