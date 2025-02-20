// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/Resources/BaseCrop.h"

#include "Kismet/KismetMathLibrary.h"
#include "Save/JHGameInstanceInterface.h"

void ABaseCrop::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	TArray<FName> MakeTag;
	MakeTag.Add("Farming");
	MakeTag.Add("Ready");
	Tags = MakeTag;
}

float ABaseCrop::Interact()
{
	float Delay;
	int32 Stage;
	FarmingProgress(Delay,Stage);
	return Delay;
}

void ABaseCrop::FarmingProgress(float& OutDelay, int32& Stage)
{
	
	Tags.RemoveAt(1);
	GetWorld()->GetTimerManager().SetTimer(Timer,this,&ABaseCrop::SwitchStage,CollectionTime,false);

	int32 TruncatedProgress = FMath::TruncToFloat(ProgressionState);
	if ( TruncatedProgress >= MeshList.Num() - 1)
	{
		TruncatedProgress = 0;
	}
	
	ProgressionState = float(TruncatedProgress);

	OutDelay = CollectionTime;
	Stage = TruncatedProgress;

}

void ABaseCrop::SwitchStage()
{
	if (0 == FMath::Floor(ProgressionState))
	{
		SetReady();
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(Timer,this,&ABaseCrop::SetReady,CollectionTime,false);
	}
}

void ABaseCrop::SetReady()
{
	FName NewTag = "";
	if (MeshList.Num()-1 == FMath::Floor(ProgressionState))
	{
		NewTag = FName("Harvest");
	}
	else
	{
		NewTag = FName("Ready");
	}

	if (Tags.Num() <= 1)
	{
		Tags.SetNum(2); 
	}
	Tags[1] = NewTag;

	Mesh->SetStaticMesh(MeshList[FMath::TruncToFloat(ProgressionState)]);

	if (IJHGameInstanceInterface* Interface = Cast<IJHGameInstanceInterface>(GetGameInstance()))
	{
		Interface->UpdateAllInteractables();
		PopFarmPlot();
	}
	
}


