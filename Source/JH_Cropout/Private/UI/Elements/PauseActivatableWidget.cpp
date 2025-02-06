// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Elements/PauseActivatableWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"

void UPauseActivatableWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// GetDesiredFocusTarget 함수는 블프에서 구현해야 되는 BlueprintImplementableEvent함수임
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(UGameplayStatics::GetPlayerController(this,0),GetDesiredFocusTarget());

	GetDesiredFocusTarget()->SetFocus();
	UGameplayStatics::SetGamePaused(this, true);

	
	
}
