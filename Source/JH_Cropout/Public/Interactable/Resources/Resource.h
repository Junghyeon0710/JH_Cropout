// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/Interactable.h"
#include "Misc/IslandInterface.h"
#include "ResourcesTypes/ResourcesTypes.h"
#include "Resource.generated.h"

/**
 * 
 */
UCLASS()
class JH_CROPOUT_API AResource : public AInteractable ,public IIslandInterface
{
	GENERATED_BODY()
public:
	//*IIslandInterface */
	virtual void ScaleUp(float Delay) override;
	//* ~IIslandInterface */

	virtual void OnConstruction(const FTransform& Transform) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartAnimation();
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EResourceType ResourceType = EResourceType::Food;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float ResourceAmount = 100.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float CollectionTime = 3.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 CollectionValue = 10;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bUseRandomMesh = false;
};
