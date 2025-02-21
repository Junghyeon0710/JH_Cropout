// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/Building/EndGame.h"

#include "GameMode/JHBlueprintFunctionLibrary.h"
#include "GameMode/JHGameMode.h"

void AEndGame::ConstructionComplete()
{
	Super::ConstructionComplete();

	UJHBlueprintFunctionLibrary::GetJhGameMode(this)->EndGame(true);
}
