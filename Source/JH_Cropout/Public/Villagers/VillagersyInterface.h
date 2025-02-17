// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VillagersyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVillagersyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class JH_CROPOUT_API IVillagersyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void Action(AActor* Actor) = 0;
	virtual void ChangeJob(FName NewJob) = 0;
	virtual void ReturnToDefaultBT() = 0;
	virtual void PlayWorkAnim(float Delay) = 0;
};
