// Fill out your copyright notice in the Description page of Project Settings.


#include "Villagers/AI/Tasks/BTT_Work.h"

#include "AIController.h"
#include "Interactable/Interactable.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameMode/ResourceInterface.h"
#include "Villagers/VillagersyInterface.h"

EBTNodeResult::Type UBTT_Work::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//If both values are valid, continue, otherwise return fail
	 UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	bool bValidTarget = IsValid( BlackboardComponent->GetValueAsObject(TakeFrom.SelectedKeyName));
	bool bSelfActor = IsValid( BlackboardComponent->GetValueAsObject(GiveTo.SelectedKeyName));

	if (!bValidTarget || !bSelfActor)
	{
		return	EBTNodeResult::Failed;
	}

	//Play transfer event, wait for delay, transfer resources
	Interactable = Cast<AInteractable>(BlackboardComponent->GetValueAsObject(TakeFrom.SelectedKeyName));

	Interactable->PlayWobble(AIOwner->GetPawn()->GetActorLocation());
	float Interact = Interactable->Interact();
	if (IVillagersyInterface* Interface = Cast<IVillagersyInterface>(AIOwner->GetPawn()))
	{
		Interface->PlayWorkAnim(Interact);
	}
	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer,FTimerDelegate::CreateWeakLambda(this,[this,BlackboardComponent]()
	{
		if (IResourceInterface* Interface = Cast<IResourceInterface>(BlackboardComponent->GetValueAsObject(TakeFrom.SelectedKeyName)))
		{
			EResourceType Resource;
			int32 Value;
			Interface->RemoveResource(Resource,Value);
			if (IResourceInterface* ResourceInterface = Cast<IResourceInterface>(BlackboardComponent->GetValueAsObject(GiveTo.SelectedKeyName)))
			{
				ResourceInterface->AddResource(Resource,Value);
			}
		}
		// OwnerComp를 const로 전달할 수 있도록 수정
		FinishExecute(true);
	}),Interact+.1,false);
	
	return	EBTNodeResult::InProgress;
	
}

EBTNodeResult::Type UBTT_Work::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Interactable->EndWobble();

	return EBTNodeResult::Failed;
}
