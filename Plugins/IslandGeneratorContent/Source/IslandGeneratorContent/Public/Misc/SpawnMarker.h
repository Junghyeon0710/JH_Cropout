// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnMarker.generated.h"

UCLASS()
class ISLANDGENERATORCONTENT_API ASpawnMarker : public AActor
{
	GENERATED_BODY()
	
public:	

	ASpawnMarker();

protected:
	virtual void BeginPlay() override;



};
