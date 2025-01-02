// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TransitionWidget.generated.h"

/**
 * 
 */
UCLASS()
class JH_CROPOUT_API UTransitionWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void TransIn();
	void TransOut();
protected:
	
	UPROPERTY(Transient , meta=(BindWidgetAnim), BlueprintReadWrite)
	TObjectPtr<UWidgetAnimation> NewAnimation;
};
