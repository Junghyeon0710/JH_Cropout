// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Villagers/AI/Tasks/BTT_DefaultBT.h"
#include "BTT_StuckRecover.generated.h"

/**
 * 
 */
UCLASS()
class JH_CROPOUT_API UBTT_StuckRecover : public UBTT_DefaultBT
{
	GENERATED_BODY()
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FBlackboardKeySelector RecoveryPosition;
};
