// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/Resources/Resource.h"
#include "BaseCrop.generated.h"

/**
 * 
 */
UCLASS()
class JH_CROPOUT_API ABaseCrop : public AResource
{
	GENERATED_BODY()
protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual float Interact() override;
	virtual void SetProgressionState(float Progression) override;

	void FarmingProgress(float& OutDelay, int32& Stage);
	void SwitchStage();
	void SetReady();
	
	UFUNCTION(BlueprintImplementableEvent)
	void PopFarmPlot();

	FTimerHandle Timer;
};
