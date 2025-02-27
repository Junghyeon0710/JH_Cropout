// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/Building/House.h"

#include "Components/BoxComponent.h"
#include "GameMode/JHBlueprintFunctionLibrary.h"
#include "GameMode/JHGameInstance.h"
#include "GameMode/JHGameMode.h"

AHouse::AHouse()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("NavBlocker"));
	BoxComponent->SetupAttachment(Mesh);
}

void AHouse::ConstructionComplete()
{
	Super::ConstructionComplete();
	SpawnVillagers();
}

void AHouse::PlacementMode()
{
	Super::PlacementMode();
	BoxComponent->DestroyComponent();
}

void AHouse::SpawnVillagers() const
{
	static bool bDoOnce = false;
	if (!bDoOnce)
	{
		UJHBlueprintFunctionLibrary::GetJhGameMode(this)->SpawnVillagers(VillagerCount);
		bDoOnce = true;
	}
	
}
