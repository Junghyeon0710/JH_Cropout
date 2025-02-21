// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResourcesTypes/ResourcesTypes.h"
#include "UObject/Interface.h"
#include "ResourceInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UResourceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class JH_CROPOUT_API IResourceInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void AddResource(EResourceType Resource, int32 Value);
	virtual void RemoveResource(EResourceType& OutResource, int32& OutValue);
	virtual bool CheckResource(EResourceType Resource, int32& OutValue);
	virtual void RemoveTargetResource(EResourceType Resource, int32 InValue);
	virtual TMap<EResourceType,int32> GetCurrentResources();
};
