// Fill out your copyright notice in the Description page of Project Settings.


#include "Villagers/AI/Tasks/BTT_TransferResource.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameMode/ResourceInterface.h"

EBTNodeResult::Type UBTT_TransferResource::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//If both values are valid, continue, otherwise return fail
	UBlackboardComponent* BBC = OwnerComp.GetBlackboardComponent();

	if (!IsValid(BBC->GetValueAsObject(TakeFrom.SelectedKeyName)) || !IsValid(BBC->GetValueAsObject(GiveTo.SelectedKeyName)))
	{
		return EBTNodeResult::Failed;
	}
	
	//transfer resources
	if (IResourceInterface* ResourceInterface = Cast<IResourceInterface>(BBC->GetValueAsObject(TakeFrom.SelectedKeyName)))
	{
		EResourceType Resource;
		int32 Value;
		ResourceInterface->RemoveResource(Resource,Value);

		if (IResourceInterface* Interface = Cast<IResourceInterface>(BBC->GetValueAsObject(GiveTo.SelectedKeyName)))
		{
			ResourceInterface->AddResource(Resource,Value);
		}
	}

	return EBTNodeResult::Succeeded;
}
