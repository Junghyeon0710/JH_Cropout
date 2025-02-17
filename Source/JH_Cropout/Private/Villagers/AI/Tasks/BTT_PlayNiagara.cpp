// Fill out your copyright notice in the Description page of Project Settings.


#include "Villagers/AI/Tasks/BTT_PlayNiagara.h"

#include "AIController.h"
#include "NiagaraFunctionLibrary.h"

EBTNodeResult::Type UBTT_PlayNiagara::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (System)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			System,
			AIOwner->GetPawn()->GetRootComponent(),
			"",
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::Type::KeepRelativeOffset,
			false);
		
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
