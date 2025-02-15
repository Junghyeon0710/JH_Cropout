// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Elements/BuildConfirmActivatableWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Player/JHPlayerController.h"
#include "Player/JH_Player.h"
#include "CommonUI/Public/CommonBorder.h"
#include "Interactable/Interactable.h"
#include "Kismet/KismetMathLibrary.h"


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

	float PositionX, PositionY;
	GetScreenLockedPosition(PositionX,PositionY);
	FWidgetTransform Transform;
	Transform.Translation.X = PositionX;
	Transform.Translation.Y = PositionY;
	CommonBorder_1->SetRenderTransform(Transform);
}

void UBuildConfirmActivatableWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const AJHPlayerController* PC = Cast<AJHPlayerController>(UGameplayStatics::GetPlayerController(this,0));

	if (!PC)
	{
		return;
	}

	const AJH_Player* JHPawn = Cast<AJH_Player>(PC->GetPawn());

	if (!JHPawn)
	{
		return;
	}

	
	if(IsValid(JHPawn->Spawn))
	{
		FVector2D CurrentTranslation = CommonBorder_1->GetRenderTransform().Translation;

		float TargetX, TargetY;
		GetScreenLockedPosition(TargetX,TargetY);
		FVector TargetVector = UKismetMathLibrary::VectorSpringInterp(FVector(CurrentTranslation.X,CurrentTranslation.Y,0.f),
			FVector(TargetX,TargetY,0.f),
			SpringState,
			50.f,
			0.9f,
			InDeltaTime);

		FWidgetTransform Transform;
		Transform.Translation.X = TargetVector.X;
		Transform.Translation.Y = TargetVector.Y;
		CommonBorder_1->SetRenderTransform(Transform);
	}
}

void UBuildConfirmActivatableWidget::UI_GameMain_AutoGenFunc(EInputType NewInput)
{
	AJHPlayerController* PC = Cast<AJHPlayerController>(UGameplayStatics::GetPlayerController(this,0));

	if (!PC)
	{
		return;
	}
	PC->bShowMouseCursor = false;

	if (NewInput == EInputType::Unknown || NewInput == EInputType::Touch)
	{
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PC,nullptr,EMouseLockMode::DoNotLock,false);
	}
	else if (NewInput == EInputType::KeyMouse)
	{
		PC->bShowMouseCursor = true;
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PC,nullptr,EMouseLockMode::DoNotLock,false);
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
	UGameplayStatics::ProjectWorldToScreen(PC,JHPawn->Spawn->GetActorLocation(),ScreenPosition,true);

	float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(this);
	FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(this);

	//Lock range to keep on screen
	ScreenPosition /= ViewportScale;
	ViewportSize /= ViewportScale;

	PositionX = FMath::Clamp(ScreenPosition.X,150.f,ViewportSize.X -150);
	PositionY = FMath::Clamp(ScreenPosition.Y,0,ViewportSize.Y -350);
}