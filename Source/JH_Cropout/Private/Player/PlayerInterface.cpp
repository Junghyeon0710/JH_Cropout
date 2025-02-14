// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerInterface.h"

void IPlayerInterface::AddUI(TSubclassOf<UCommonActivatableWidget> Widget)
{
	
}

void IPlayerInterface::SwitchBuildMode(bool bIsInBuildMode)
{
}

void IPlayerInterface::BeginBuild(const TSubclassOf<AInteractable>& TargetClass,
	const TMap<EResourceType, int32>& ResourceCost)
{
}
