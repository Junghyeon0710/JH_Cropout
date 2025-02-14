// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Elements/BuildConfirmActivatableWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Player/JHPlayerController.h"
#include "Player/JH_Player.h"


void UBuildConfirmActivatableWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	AJHPlayerController* PC = Cast<AJHPlayerController>(UGameplayStatics::GetPlayerController(this,0));

	if (!PC)
	{
		return;
	}

	UI_GameMain_AutoGenFunc(PC->InputType);
	PC->OnCallKeySwitch.AddUObject(this,&ThisClass::UI_GameMain_AutoGenFunc);

	PC->GetPawn()->EnableInput(PC);
	PC->GetPawn()->SetActorTickEnabled(true);
	UWidgetBlueprintLibrary::SetFocusToGameViewport();

	
}

void UBuildConfirmActivatableWidget::UI_GameMain_AutoGenFunc(EInputType NewInput) const
{
	AJHPlayerController* PC = Cast<AJHPlayerController>(UGameplayStatics::GetPlayerController(this,0));

	if (!PC)
	{
		return;
	}
	PC->bShowMouseCursor = false;

	if (NewInput == EInputType::Unknown || NewInput == EInputType::Touch)
	{
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PC);
	}
	else if (NewInput == EInputType::KeyMouse)
	{
		PC->bShowMouseCursor = true;
	}
	else
	{
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(PC);
	}
	
	UWidgetBlueprintLibrary::SetFocusToGameViewport();
}

void UBuildConfirmActivatableWidget::GetScreenLockedPosition(float& PositionX, float& PositionY)
{
	//Target position relative to viewport scaled viewport size
	AJHPlayerController* PC = Cast<AJHPlayerController>(UGameplayStatics::GetPlayerController(this,0));

	if (!PC)
	{
		return;
	}

	AJH_Player* JHPawn = Cast<AJH_Player>(PC->GetPawn());

	if (!JHPawn)
	{
		return;
	}

	FVector2D ScreenPosition;
	UGameplayStatics::ProjectWorldToScreen(PC,JHPawn->GetActorLocation(),ScreenPosition,true);

	float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(this);
	FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(this);

	//Lock range to keep on screen
	ScreenPosition /= ViewportScale;
	ViewportSize /= ViewportScale;

	PositionX = FMath::Clamp(ScreenPosition.X,150.f,ViewportSize.X -150);
	PositionY = FMath::Clamp(ScreenPosition.Y,150.f,ViewportSize.Y -350);
}