// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/Building/BuildingBase.h"
#include "House.generated.h"

/**
 * 
 */
UCLASS()
class JH_CROPOUT_API AHouse : public ABuildingBase
{
	GENERATED_BODY()
public:
	AHouse();
	
	virtual void ConstructionComplete() override;
	virtual void PlacementMode() override;
	void SpawnVillagers() const;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(EditAnywhere)
	int32 VillagerCount;
};
