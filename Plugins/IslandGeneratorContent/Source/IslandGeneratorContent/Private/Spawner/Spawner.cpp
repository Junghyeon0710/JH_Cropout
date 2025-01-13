// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner/Spawner.h"

#include "Engine/AssetManager.h"


ASpawner::ASpawner()
{

	PrimaryActorTick.bCanEverTick = false;

}

void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	AsyncLoadClasses();
}

void ASpawner::AsyncLoadClasses()
{
	ClassRefIndex = 0;
	bAsyncComplete = false;

	SpawnTypes[ClassRefIndex].ClassRef.Get();
	
	FSoftObjectPath SoftObjectPath(SpawnTypes[ClassRefIndex].ClassRef.ToSoftObjectPath());
	FStreamableDelegate StreamableDelegate;
	UAssetManager::GetStreamableManager().RequestAsyncLoad(SoftObjectPath, FStreamableDelegate::CreateLambda([this, SoftObjectPath]()
	{
		ClassRefIndex++;
		if (ClassRefIndex>SpawnTypes.Num())
		{
			bAsyncComplete = true;
		}
		else
		{
			AsyncLoadClasses();
		}
   }));
	
}


