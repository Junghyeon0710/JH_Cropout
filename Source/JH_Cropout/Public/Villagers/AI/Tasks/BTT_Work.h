// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTT_Work.generated.h"

class AInteractable;
/**
 * 
 */
UCLASS()
class JH_CROPOUT_API UBTT_Work : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector DelayKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector GiveTo;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TakeFrom;

	UPROPERTY(EditAnywhere)
	float DelayMultiplier;

	UPROPERTY()
	TObjectPtr<AInteractable> Interactable;
};
