// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "JHSaveInteract.generated.h"

class AInteractable;

USTRUCT()
struct FSaveInteract
{
	GENERATED_BODY()

	UPROPERTY()
	FTransform Location;

	UPROPERTY()
	TSubclassOf<AInteractable> Type;

	UPROPERTY()
	float Health;

	UPROPERTY()
	FName Tag;
};
