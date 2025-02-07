// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "EndGameActivatableWidget.generated.h"

class USoundControlBus;
class UJHCommonButtonBase;
class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class JH_CROPOUT_API UEndGameActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnActivated() override;
	void EndGame(bool InWin);
private:
	UPROPERTY(meta=(BindWidget) ,EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	TObjectPtr<UCommonTextBlock> MainText;

	UPROPERTY(meta=(BindWidget) ,EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	TObjectPtr<UJHCommonButtonBase> BTN_Retry;

	UPROPERTY(meta=(BindWidget) ,EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	TObjectPtr<UJHCommonButtonBase> BTN_Continue;

	UPROPERTY(meta=(BindWidget) ,EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	TObjectPtr<UJHCommonButtonBase> BTN_MainMenu;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	bool bIsWin;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	FText WinText;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	FText LoseText;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	TObjectPtr<USoundControlBus> WinControlBus;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	TObjectPtr<USoundControlBus> LoseControlBus;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	TSoftObjectPtr<UWorld> MainMenuLevel;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	TSoftObjectPtr<UWorld> VillageLevel;
};
