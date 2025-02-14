// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResourcesTypes/ResourcesTypes.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

class AInteractable;
class UCommonActivatableWidget;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class JH_CROPOUT_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void AddUI(TSubclassOf<UCommonActivatableWidget> Widget);
	virtual void SwitchBuildMode(bool bIsInBuildMode);
	virtual void BeginBuild(const TSubclassOf<AInteractable>& TargetClass,const TMap<EResourceType,int32>& ResourceCost);
};
