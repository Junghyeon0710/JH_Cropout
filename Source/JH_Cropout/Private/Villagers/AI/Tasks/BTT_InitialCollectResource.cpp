// Fill out your copyright notice in the Description page of Project Settings.


#include "Villagers/AI/Tasks/BTT_InitialCollectResource.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interactable/Building/TownCenter.h"
#include "Interactable/Resources/Resource.h"
#include "Kismet/GameplayStatics.h"
#include "Villagers/JH_Villager.h"

EBTNodeResult::Type UBTT_InitialCollectResource::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Check if any remaining resources available to gather
	TArray<AActor*> Resources; 
	UGameplayStatics::GetAllActorsOfClassWithTag(this,AResource::StaticClass(),AIOwner->GetPawn()->Tags[0],Resources);

	if(Resources.IsEmpty())
	{
		return EBTNodeResult::Failed;
	}
	UBlackboardComponent* BBC = OwnerComp.GetBlackboardComponent();
	BBC->SetValueAsName(Key_ResourceTag.SelectedKeyName,AIOwner->GetPawn()->Tags[0]);

	//Set Target Class reference
	BBC->SetValueAsClass(Key_ResourceClass.SelectedKeyName,AResource::StaticClass());
	
	//Check if a valid target is already stored
	APawn* AIPawn = AIOwner->GetPawn();

	AJH_Villager* Villager = Cast<AJH_Villager>(AIPawn);
	if(IsValid(Villager->TargetRef))
	{
		BBC->SetValueAsObject(Key_Resource.SelectedKeyName,Villager->TargetRef);
	}

	//Store Ref to town hall
	TArray<AActor*> TownCenter; 
	UGameplayStatics::GetAllActorsOfClass(this,ATownCenter::StaticClass(),TownCenter);
	BBC->SetValueAsObject(Key_TownHall.SelectedKeyName,TownCenter[0]);
	
	return EBTNodeResult::Succeeded;
}
