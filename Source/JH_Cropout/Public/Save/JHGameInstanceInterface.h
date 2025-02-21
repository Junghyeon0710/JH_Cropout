// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResourcesTypes/ResourcesTypes.h"
#include "UObject/Interface.h"
#include "JHGameInstanceInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UJHGameInstanceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class JH_CROPOUT_API IJHGameInstanceInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool CheckSaveBool() = 0;
	virtual void ClearSave(bool ClearSeed) = 0;
	virtual void SaveGame() = 0;
	virtual void UpdateAllInteractables() = 0 ;
	virtual void UpdateAllResources(TMap<EResourceType,int32> NewParam) = 0 ;
	virtual void UpdateAllVillagers() = 0;
};
