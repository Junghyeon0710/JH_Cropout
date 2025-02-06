// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Input/InputType.h"
// #include "EnhancedPlayerInput.h"
// #include "InputAction.h"

#include "JHPlayerController.generated.h"

/**
 * 
 */

DECLARE_DELEGATE_OneParam(FOnCallKeySwitch,EInputType NewInput)

struct FInputActionInstance;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

UCLASS()
class JH_CROPOUT_API AJHPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> DetectInputMappingContext;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> KeyDetectAction;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> MouseMove;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> TouchDetect;

	
	void InputActionKeyDetect(const FInputActionInstance& ActionInstance);
	void InputAxisMouseMove(const FInputActionValue& Value);
	void InputActionTouchDetect(const FInputActionInstance& ActionInstance);

	EInputType InputType;
public:
	FOnCallKeySwitch OnCallKeySwitch;
};


