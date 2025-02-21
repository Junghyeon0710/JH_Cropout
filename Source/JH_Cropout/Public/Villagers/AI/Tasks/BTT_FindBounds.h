// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Villagers/AI/Tasks/BTT_DefaultBT.h"
#include "BTT_FindBounds.generated.h"

/**
 * 
 */
UCLASS()
class JH_CROPOUT_API UBTT_FindBounds : public UBTT_DefaultBT
{
	GENERATED_BODY()
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FBlackboardKeySelector Target;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FBlackboardKeySelector BBBound;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float AdditionalBounds;
};
