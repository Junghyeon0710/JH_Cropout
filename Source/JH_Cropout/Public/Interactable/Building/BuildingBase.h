// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/Interactable.h"
#include "BuildingBase.generated.h"

/**
 * 
 */
UCLASS()
class JH_CROPOUT_API ABuildingBase : public AInteractable
{
	GENERATED_BODY()
public:
	virtual float Interact() override;

	float ProgressConstruct(float InvestedTime);

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	int32 CurrentStage;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float BuildDifficulty;

	virtual void SpawnInBuildMode(float Progression);
	virtual void ConstructionComplete();
	
};
