// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "JH_Player.generated.h"

class USphereComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class JH_CROPOUT_API AJH_Player : public APawn
{
	GENERATED_BODY()

public:

	AJH_Player();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<USceneComponent> RootComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> Cursor;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<USphereComponent> Collision;
public:	


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
};
