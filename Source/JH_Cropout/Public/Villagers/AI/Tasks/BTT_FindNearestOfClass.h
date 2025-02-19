// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTT_FindNearestOfClass.generated.h"

/**
 * 
 */
UCLASS()
class JH_CROPOUT_API UBTT_FindNearestOfClass : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
	
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere,Category = Class)
	bool bUseBlackboardClass = false;
	
	UPROPERTY(EditAnywhere,Category = Class)
	FBlackboardKeySelector TargetClass;

	UPROPERTY(EditAnywhere,Category = Class)
	TSubclassOf<UObject> ManualClass;

	UPROPERTY(EditAnywhere,Category = Tag)
	FName TagFilter;

	UPROPERTY(EditAnywhere,Category = Class)
	bool bUseBlackboardTag;

	UPROPERTY(EditAnywhere,Category = Class)
	FBlackboardKeySelector BlackboardTag;
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Target;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector NearestTo;

	UPROPERTY()
	TArray<AActor*> PossibleActors;

	bool bPathFound;

	UPROPERTY()
	AActor* NewTarget;
};
