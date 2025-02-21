// Fill out your copyright notice in the Description page of Project Settings.


#include "Villagers/AI/Tasks/BTT_ProgressConstruction.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interactable/Interactable.h"
#include "Villagers/VillagersyInterface.h"

EBTNodeResult::Type UBTT_ProgressConstruction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	 UBlackboardComponent* BBC = OwnerComp.GetBlackboardComponent();
	if (IVillagersyInterface* VillagersInterface = Cast<IVillagersyInterface>(AIOwner->GetPawn()))
	{
		VillagersInterface->PlayWorkAnim(1.f);
	}

	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer,FTimerDelegate::CreateWeakLambda(this,[this,BBC]()
	{
		if (AInteractable* Interactable = Cast<AInteractable>(BBC->GetValueAsObject(TargetBuild.SelectedKeyName)))
		{
			const float Delay = Interactable->Interact();

			if (Delay <= 0.f)
			{
				BBC->SetValueAsObject(TargetBuild.SelectedKeyName,nullptr);
			}
			FinishExecute(true);
		}
	}),1.1f,false);
	
	return EBTNodeResult::InProgress;
}
