// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ResourceInterface.h"


void IResourceInterface::AddResource(EResourceType Resource, int32 Value)
{
}

void IResourceInterface::RemoveResource(EResourceType& OutResource, int32& OutValue)
{
}

bool IResourceInterface::CheckResource(EResourceType Resource, int32& OutValue)
{
	return false;
}

void IResourceInterface::RemoveTargetResource(EResourceType Resource, int32 InValue)
{
	
}

TMap<EResourceType, int32> IResourceInterface::GetCurrentResources()
{
	return TMap<EResourceType,int32>();
}
