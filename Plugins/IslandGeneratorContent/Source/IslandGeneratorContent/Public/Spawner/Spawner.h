// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnData.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

UCLASS()
class ISLANDGENERATORCONTENT_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ASpawner();
	
	void AsyncLoadClasses();
protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	int32 ClassRefIndex;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	bool bAsyncComplete;

	UPROPERTY(EditAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	TArray<FSpawnData> SpawnTypes;

};
