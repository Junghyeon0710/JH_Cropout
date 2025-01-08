// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "JHMenuPawn.generated.h"

class USceneComponent;
class URotatingMovementComponent;
class UCameraComponent;
class USpringArmComponent;
class UCameraShakeSourceComponent;
UCLASS()
class JH_CROPOUT_API AJHMenuPawn : public APawn
{
	GENERATED_BODY()

public:
	AJHMenuPawn();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> RootComponentDefault;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraShakeSourceComponent> CameraShakeSource;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URotatingMovementComponent> RotatingMovement;

};
