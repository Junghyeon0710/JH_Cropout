// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "JH_Villager.generated.h"

class UFloatingPawnMovement;
class UCapsuleComponent;

UCLASS()
class JH_CROPOUT_API AJH_Villager : public APawn
{
	GENERATED_BODY()

public:

	AJH_Villager();
	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION()
	void Eat() const;
	
protected:

	virtual void BeginPlay() override;
	

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UCapsuleComponent> Capsule;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Tool;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> Hat;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> Hair;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UDecalComponent> Decal;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UFloatingPawnMovement> FloatingPawnMovement;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	int32 Quantity;

private:
	FTimerHandle EatTimer;
};
