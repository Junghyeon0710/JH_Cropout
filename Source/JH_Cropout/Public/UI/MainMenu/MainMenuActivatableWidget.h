// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "MainMenuActivatableWidget.generated.h"

class UCommonButtonBase;
class ULevel;
/**
 * 
 */
class UCommonActivatableWidgetStack;

UCLASS()
class JH_CROPOUT_API UMainMenuActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeOnActivated() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Save)
	bool bHasSave;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bDisableIAP;

	UPROPERTY(meta=(BindWidget), BlueprintReadWrite )
	TObjectPtr<UCommonButtonBase> BTN_Continue;

	UPROPERTY(meta=(BindWidget), BlueprintReadWrite )
	TObjectPtr<UCommonButtonBase> BTN_NewGame;

	UPROPERTY(meta=(BindWidget), BlueprintReadWrite )
	TObjectPtr<UCommonButtonBase> BTN_Quit;

	UPROPERTY(meta=(BindWidget), BlueprintReadWrite )
	TObjectPtr<UCommonButtonBase> BTN_Donate;

	
public:
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UCommonActivatableWidgetStack> StackRef;

	UPROPERTY(EditDefaultsOnly)
	bool Test;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> Level;
};
