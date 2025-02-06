// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JHSaveInteract.h"
#include "Villager.h"
#include "GameFramework/SaveGame.h"
#include "ResourcesTypes/ResourcesTypes.h"
#include "JHSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class JH_CROPOUT_API UJHSaveGame : public USaveGame
{
	GENERATED_BODY()
public:

	UPROPERTY()
	FRandomStream Seed;

	UPROPERTY()
	TMap<EResourceType,int32> Resources;

	UPROPERTY()
	TArray<FSaveInteract> Interactables;

	UPROPERTY()
	TArray<FVillager> Villagers;
	
	UPROPERTY()
	float PlayTime;
};
