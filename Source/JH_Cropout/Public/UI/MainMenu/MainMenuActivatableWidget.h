// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "MainMenuActivatableWidget.generated.h"

/**
 * 
 */
class UCommonActivatableWidgetStack;

UCLASS()
class JH_CROPOUT_API UMainMenuActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UCommonActivatableWidgetStack> StackRef;
};
