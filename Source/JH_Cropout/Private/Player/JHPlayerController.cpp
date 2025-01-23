// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/JHPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/InputDeviceSubsystem.h"

void AJHPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* LS = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		LS->AddMappingContext(DetectInputMappingContext,0);
	}
}

void AJHPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(KeyDetectAction,ETriggerEvent::Started,this,&AJHPlayerController::InputActionKeyDetect);
		EnhancedInputComponent->BindAction(MouseMove,ETriggerEvent::Triggered,this,&AJHPlayerController::InputAxisMouseMove);
		EnhancedInputComponent->BindAction(TouchDetect,ETriggerEvent::Started,this,&AJHPlayerController::InputActionTouchDetect);
	}
}

void AJHPlayerController::InputActionKeyDetect(const FInputActionInstance& ActionInstance)
{
	if(UInputDeviceSubsystem* InputDeviceSubsystem = UInputDeviceSubsystem::Get())
	{
		if (EHardwareDevicePrimaryType::Gamepad == InputDeviceSubsystem->GetMostRecentlyUsedHardwareDevice(FPlatformUserId()).PrimaryDeviceType)
		{
			if (InputType != EInputType::Gamepad)
			{
				InputType = EInputType::Gamepad;
				GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,TEXT("GamePad"));
				if (OnCallKeySwitch.IsBound())
				{
					OnCallKeySwitch.Execute(InputType);
				}
			}
		}
	}
}

void AJHPlayerController::InputAxisMouseMove(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D AxisValue = Value.Get<FVector2D>();
	if (AxisValue.IsZero()) return;

	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,TEXT("MouseMove"));
	if (InputType != EInputType::KeyMouse)
	{
		InputType = EInputType::KeyMouse;
		if (OnCallKeySwitch.IsBound())
		{
			OnCallKeySwitch.Execute(InputType);
		}
	}
}

void AJHPlayerController::InputActionTouchDetect(const FInputActionInstance& ActionInstance)
{
	if (InputType != EInputType::Touch)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,TEXT("Touch"));
		InputType = EInputType::Touch;
		if (OnCallKeySwitch.IsBound())
		{
			OnCallKeySwitch.Execute(InputType);
		}
	}
}