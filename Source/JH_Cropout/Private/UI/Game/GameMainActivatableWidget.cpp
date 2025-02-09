// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Game/GameMainActivatableWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Player/JHPlayerController.h"



void UGameMainActivatableWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	//Set UI input mode. Switch between Game and UI and UI only to avoid common UI eating gamepad input.
	AJHPlayerController* PC = Cast<AJHPlayerController>(UGameplayStatics::GetPlayerController(this,0));

	UIGameMainAutoGenFunc(PC->InputType);

	PC->OnCallKeySwitch.AddUObject(this,&ThisClass::UIGameMainAutoGenFunc);
	PC->GetPawn()->EnableInput(PC);
	PC->GetPawn()->SetActorTickEnabled(true);
	UWidgetBlueprintLibrary::SetFocusToGameViewport();
	
}

void UGameMainActivatableWidget::UIGameMainAutoGenFunc(EInputType NewType)
{
	APlayerController* PC =  UGameplayStatics::GetPlayerController(this,0);
	PC->bShowMouseCursor = false;

	if(NewType == EInputType::Gamepad)
	{
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(PC,true);
	}
	else if(NewType == EInputType::KeyMouse)
	{
		PC->bShowMouseCursor = true;
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PC,nullptr,EMouseLockMode::DoNotLock,false);
	}
	else
	{
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PC,nullptr,EMouseLockMode::DoNotLock,false);
	}
	UWidgetBlueprintLibrary::SetFocusToGameViewport();
	
}