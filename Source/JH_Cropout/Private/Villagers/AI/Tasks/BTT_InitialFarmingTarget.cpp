// Fill out your copyright notice in the Description page of Project Settings.


#include "Villagers/AI/Tasks/BTT_InitialFarmingTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interactable/Building/BuildingBase.h"
#include "Interactable/Building/TownCenter.h"
#include "Interactable/Resources/BaseCrop.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UBTT_InitialFarmingTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Has a target resource already been set?
	UBlackboardComponent* BBC = OwnerComp.GetBlackboardComponent();
	if (IsValid( BBC->GetValueAsObject(Key_Resource.SelectedKeyName)))
	{
		//Set Classes, used to find new asset types when original is nno longer available.
		BBC->SetValueAsClass(Key_ResourceClass.SelectedKeyName,ABaseCrop::StaticClass());
		BBC->SetValueAsClass(Key_CollectionClass.SelectedKeyName,ABuildingBase::StaticClass());

		//Store a backup of the town hall
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(this,ATownCenter::StaticClass(),Actors);
		BBC->SetValueAsObject(Key_TownHall.SelectedKeyName,Actors[0]);

		return EBTNodeResult::Succeeded;
	}

	
	//Find resource to farm
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClassWithTag(this,ABaseCrop::StaticClass(),FName("Readay"),Actors);

	if (!Actors.IsValidIndex(0))
	{
		return EBTNodeResult::Failed;
	}

	float Distance;
	AActor* NearestActor = UGameplayStatics::FindNearestActor(AIOwner->GetPawn()->GetActorLocation(),Actors,Distance);

	BBC->SetValueAsObject(Key_Resource.SelectedKeyName,NearestActor);

	//Set Classes, used to find new asset types when original is nno longer available.
	BBC->SetValueAsClass(Key_ResourceClass.SelectedKeyName,ABaseCrop::StaticClass());
	BBC->SetValueAsClass(Key_CollectionClass.SelectedKeyName,ABuildingBase::StaticClass());

	//Store a backup of the town hall
	TArray<AActor*> TownCenters;
	UGameplayStatics::GetAllActorsOfClass(this,ATownCenter::StaticClass(),TownCenters);
	BBC->SetValueAsObject(Key_TownHall.SelectedKeyName,TownCenters[0]);
	
	return EBTNodeResult::Succeeded;
}
