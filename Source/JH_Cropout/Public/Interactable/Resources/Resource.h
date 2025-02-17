// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/ResourceInterface.h"
#include "Interactable/Interactable.h"
#include "Misc/IslandInterface.h"
#include "ResourcesTypes/ResourcesTypes.h"
#include "Resource.generated.h"

/**
 * 
 */
UCLASS()
class JH_CROPOUT_API AResource : public AInteractable ,public IIslandInterface, public IResourceInterface
{
	GENERATED_BODY()
public:
	//*IIslandInterface */
	virtual void ScaleUp(float Delay) override;
	//* ~IIslandInterface */

	//*IResourceInterface */
	virtual void RemoveResource(EResourceType& OutResource, int32& OutValue) override;
	//* ~IResourceInterface */

	virtual void OnConstruction(const FTransform& Transform) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartAnimation();

	void Death();
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
