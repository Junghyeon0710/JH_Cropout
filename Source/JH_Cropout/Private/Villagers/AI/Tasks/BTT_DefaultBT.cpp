// Fill out your copyright notice in the Description page of Project Settings.


#include "Villagers/AI/Tasks/BTT_DefaultBT.h"

#include "AIController.h"
#include "Villagers/VillagersyInterface.h"

EBTNodeResult::Type UBTT_DefaultBT::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (IVillagersyInterface* Interface = Cast<IVillagersyInterface>(AIOwner->GetPawn()))
	{
		Interface->ReturnToDefaultBT();

		return EBTNodeResult::Succeeded;
	}
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
