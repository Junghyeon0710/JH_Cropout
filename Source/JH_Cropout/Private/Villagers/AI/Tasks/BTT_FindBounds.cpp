// Fill out your copyright notice in the Description page of Project Settings.


#include "Villagers/AI/Tasks/BTT_FindBounds.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTFunctionLibrary.h"

EBTNodeResult::Type UBTT_FindBounds::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BBC = OwnerComp.GetBlackboardComponent();

	
	AActor* TargetActor = UBTFunctionLibrary::GetBlackboardValueAsActor(this,Target);

	if (!IsValid(TargetActor))
	{
		return EBTNodeResult::Failed;
	}

	FVector Origin;
	FVector BoxExtent;
	TargetActor->GetActorBounds(false,Origin,BoxExtent);
	
	BBC->SetValueAsFloat(BBBound.SelectedKeyName,FMath::Min(BoxExtent.X,BoxExtent.Y) + AdditionalBounds);

	return EBTNodeResult::Succeeded;
}
