// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "PromptActivatableWidget.generated.h"

DECLARE_DELEGATE(FClearBindingSignature)

class UJHCommonButtonBase;
class UCommonTextBlock;


/**
 * 
 */
UCLASS()
class JH_CROPOUT_API UPromptActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnActivated() override;

	UPROPERTY(meta=(BindWidget) , BlueprintReadOnly)
	TObjectPtr<UCommonTextBlock> Title;

	UPROPERTY(meta=(BindWidget) , BlueprintReadOnly)
	TObjectPtr<UJHCommonButtonBase> BTN_Pos;

	UPROPERTY(meta=(BindWidget) , BlueprintReadOnly)
	TObjectPtr<UJHCommonButtonBase> BTN_Neg;

	
public:
	FText PromptQuestion;

	void ClearBindings();

	FClearBindingSignature OnCallConfirm;
	FClearBindingSignature OnCallBack;
	
};
