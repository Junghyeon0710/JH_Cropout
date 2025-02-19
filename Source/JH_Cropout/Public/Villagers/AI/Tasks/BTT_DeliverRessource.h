// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTT_DeliverRessource.generated.h"

/**
 * 
 */
UCLASS()
class JH_CROPOUT_API UBTT_DeliverRessource : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TakeFrom;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector GiveTo;
};
