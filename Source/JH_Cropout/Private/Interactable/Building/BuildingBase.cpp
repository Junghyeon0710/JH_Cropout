// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/Building/BuildingBase.h"

#include "Save/JHGameInstanceInterface.h"

float ABuildingBase::Interact()
{
	return ProgressConstruct(0.4f);
}

float ABuildingBase::ProgressConstruct(float InvestedTime)
{
	//Update Current Build Progression
	ProgressionState = InvestedTime / BuildDifficulty + ProgressionState;

	if (ProgressionState>= MeshList.Num()-1)
	{
		// If build is complete, trigger construction complete and set final mesh
		ConstructionComplete();
		Mesh->SetStaticMesh(MeshList[MeshList.Num()-1]);
	}
	else
	{
		//If Build
		if (FMath::Floor(ProgressionState) > CurrentStage)
		{
			CurrentStage = FMath::Floor(ProgressionState);
			if (IsValid(MeshList[CurrentStage]))
			{
				Mesh->SetStaticMesh(MeshList[CurrentStage]);
			}
		}
	}

	if (IJHGameInstanceInterface* Interface = Cast<IJHGameInstanceInterface>(GetGameInstance()))
	{
		Interface->UpdateAllInteractables();
	}

	return MeshList.Num() - 1 - ProgressionState;
}

void ABuildingBase::SpawnInBuildMode(float Progression)
{
	ProgressionState = Progression;
	Tags.Add("Build");

	int32 TruncToInt = FMath::TruncToInt( ProgressionState * MeshList.Num() - 1 );

	if (IsValid(MeshList[TruncToInt]))
	{
		Mesh->SetStaticMesh(MeshList[TruncToInt]);
	}
}

void ABuildingBase::ConstructionComplete()
{
	//Once the building is constructed we can remove the build tag
	Tags.Remove("Build");

	if (IJHGameInstanceInterface* Interface = Cast<IJHGameInstanceInterface>(GetGameInstance()))
	{
		Interface->UpdateAllInteractables();
	}
}
