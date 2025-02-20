// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Villagers/AI/Tasks/BTT_DefaultBT.h"
#include "BTT_InitialFarmingTarget.generated.h"

/**
 * 
 */
UCLASS()
class JH_CROPOUT_API UBTT_InitialFarmingTarget : public UBTT_DefaultBT
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Key_ResourceClass;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Key_CollectionClass;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Key_Resource;
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Key_TownHall;
};
