// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "JHCommonButtonBase.generated.h"

class UCommonTextBlock;
class USizeBox;
class UTextBlock;
/**
 * 
 */
UCLASS()
class JH_CROPOUT_API UJHCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

protected:

	virtual void NativePreConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> ButtonTitle;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> ButtonSize;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonActionWidget> GamepadIcon;
	
	UPROPERTY(EditAnywhere, Category = Text)
	FText ButtonText;
};
