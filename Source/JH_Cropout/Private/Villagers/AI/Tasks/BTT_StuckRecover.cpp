// Fill out your copyright notice in the Description page of Project Settings.


#include "Villagers/AI/Tasks/BTT_StuckRecover.h"
#include "BehaviorTree/BehaviorTreeTypes.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTFunctionLibrary.h"

EBTNodeResult::Type UBTT_StuckRecover::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AIOwner->GetPawn()->SetActorLocation(OwnerComp.GetBlackboardComponent()->GetValueAsVector(RecoveryPosition.SelectedKeyName)))
	{
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
