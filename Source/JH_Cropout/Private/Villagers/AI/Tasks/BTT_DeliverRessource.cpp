// Fill out your copyright notice in the Description page of Project Settings.


#include "Villagers/AI/Tasks/BTT_DeliverRessource.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interactable/Interactable.h"
#include "GameMode/ResourceInterface.h"
#include "Villagers/VillagersyInterface.h"
#include "GameFramework/GameMode.h"

EBTNodeResult::Type UBTT_DeliverRessource::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//If both values are valid, continue, otherwise return fail
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	bool bValidTarget = IsValid( BlackboardComponent->GetValueAsObject(TakeFrom.SelectedKeyName));
	bool bSelfActor = IsValid( BlackboardComponent->GetValueAsObject(GiveTo.SelectedKeyName));

	if (!bValidTarget || !bSelfActor)
	{
		return	EBTNodeResult::Failed;
	}

	float Delay = 0.f;
	if(IVillagersyInterface* Interface = Cast<IVillagersyInterface>(AIOwner->GetPawn()))
	{
		Delay = Interface->PlayDeliverAnim();
	}
	
	//Play transfer event, wait for delay, transfer resources
	
	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer,FTimerDelegate::CreateWeakLambda(this,[this,BlackboardComponent]()
	{
		if (IResourceInterface* Interface = Cast<IResourceInterface>(BlackboardComponent->GetValueAsObject(TakeFrom.SelectedKeyName)))
		{
			EResourceType Resource;
			int32 Value;
			Interface->RemoveResource(Resource,Value);
			if (IResourceInterface* ResourceInterface = Cast<IResourceInterface>(GetWorld()->GetAuthGameMode()))
			{
				ResourceInterface->AddResource(Resource,Value);
			}
		}

		FinishExecute(true);
	}),Delay,false);
	
	return	EBTNodeResult::InProgress;
	
}
