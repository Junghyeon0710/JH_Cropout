// Fill out your copyright notice in the Description page of Project Settings.


#include "Villagers/AI/Tasks/BTT_ProgressFarmingSeq.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Interactable/Interactable.h"
#include "Villagers/VillagersyInterface.h"

EBTNodeResult::Type UBTT_ProgressFarmingSeq::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Progress Farm Plot
	UBlackboardComponent* BBC = OwnerComp.GetBlackboardComponent();
	
	AActor* CropActor = UBTFunctionLibrary::GetBlackboardValueAsActor(this,Crop);
	if (CropActor->ActorHasTag(FName("Ready")) || CropActor->ActorHasTag(FName("Harvest")))
	{
		TagState = CropActor->Tags[1];
		if (AInteractable* Interactable = Cast<AInteractable>(CropActor))
		{
			float Delay = Interactable->Interact();
			if (IVillagersyInterface* VillagersInterface = Cast<IVillagersyInterface>(AIOwner->GetPawn()))
			{
				VillagersInterface->PlayWorkAnim(Delay);
			}
			if (IVillagersyInterface* Interface = Cast<IVillagersyInterface>(AIOwner->GetPawn()))
			{
				FTimerHandle Timer;
				GetWorld()->GetTimerManager().SetTimer(Timer,FTimerDelegate::CreateWeakLambda(this,[this,BBC]()
				{
					if (TagState == FName("Harvest"))
					{
						FinishExecute(false);
					}
					else
					{
						//Find new crop
						BBC->SetValueAsObject(Crop.SelectedKeyName, nullptr);
						FinishExecute(true);
					}
				}),Delay+.1,false);
			}
		}
	}
	else
	{
		BBC->SetValueAsObject(Crop.SelectedKeyName, nullptr);
		FinishExecute(true);
	}
	
	//Find new crop
	//BBC->SetValueAsObject(Crop.SelectedKeyName,nullptr);
	return EBTNodeResult::InProgress;
}
