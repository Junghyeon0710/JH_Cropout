// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Input/InputType.h"
#include "GameMainActivatableWidget.generated.h"

class UBuildActivatableWidget;
class UJHCommonButtonBase;
/**
 * 
 */
UCLASS()
class JH_CROPOUT_API UGameMainActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
public:
	void UIGameMainAutoGenFunc(EInputType NewType);
protected:
	virtual void NativeOnActivated() override;
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta=(BindWidget) ,EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	TObjectPtr<UJHCommonButtonBase> CUI_Button_55;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	TSubclassOf<UCommonActivatableWidget> BuildWidgetClass;
};
