// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Villager.generated.h"

USTRUCT(BlueprintType)
struct FVillager
{
	GENERATED_BODY()

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FName Task;
};
