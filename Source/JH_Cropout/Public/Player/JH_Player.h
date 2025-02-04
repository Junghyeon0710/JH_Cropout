// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Input/InputType.h"
#include "JH_Player.generated.h"

class UCheats;
class UFloatingPawnMovement;
class USphereComponent;
class UCameraComponent;
class USpringArmComponent;
class USceneComponent;
class UCurveFloat;

USTRUCT(BlueprintType)
struct FEdgeMoveVector
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FVector Direction;

	UPROPERTY(BlueprintReadOnly)
	float Strength;
	
};

UCLASS()
class JH_CROPOUT_API AJH_Player : public APawn
{
	GENERATED_BODY()

public:

	AJH_Player();
	virtual void PossessedBy(AController* NewController) override;
	
	void UpdateZoom();
	void Dof();
	void MoveTracking();
	void UpdateCursorPosition();
	FEdgeMoveVector EdgeMove();
	void ProjcetMouseTouchToGroundPlane(FVector2d& ScreenPos, FVector& Intersection,bool& ReturnValue);
	APlayerController* GetPlayerController() const;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> Cursor;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<USphereComponent> Collision;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<UCheats> Cheats;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<UFloatingPawnMovement> FloatingPawnMovement;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<UCurveFloat> ZoomCurve;


	UPROPERTY(BlueprintReadOnly)
	FPostProcessSettings PostProcessSettings;

	UPROPERTY(BlueprintReadOnly)
	EInputType InputType;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<AActor> HoverActor;
public:	


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	
	float ZoomDirection;
	float ZoomValue = .5f;
};
