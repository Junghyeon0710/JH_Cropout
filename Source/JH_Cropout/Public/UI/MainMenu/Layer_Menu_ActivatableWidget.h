// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Layer_Menu_ActivatableWidget.generated.h"

class UMainMenuActivatableWidget;
/**
 * 
 */
class UCommonActivatableWidgetStack;
UCLASS()
class JH_CROPOUT_API ULayer_Menu_ActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeOnActivated() override;

	void AddStackItem(const TSubclassOf<UCommonActivatableWidget>& InActivatableWidgetClass);
	
	UPROPERTY(meta = (BindWidget) , BlueprintReadOnly)
	TObjectPtr<UCommonActivatableWidgetStack> MainStack;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMainMenuActivatableWidget> ActivatableWidgetClass;
};
