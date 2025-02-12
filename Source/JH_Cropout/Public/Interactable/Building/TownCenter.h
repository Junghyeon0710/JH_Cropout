// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/Building/BuildingBase.h"
#include "TownCenter.generated.h"

/**
 * 
 */
UCLASS()
class JH_CROPOUT_API ATownCenter : public ABuildingBase
{
	GENERATED_BODY()
public:
	ATownCenter();
protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> BoxComponent;
};
