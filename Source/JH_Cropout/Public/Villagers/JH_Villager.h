// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VillagersyInterface.h"
#include "GameFramework/Pawn.h"
#include "JH_Villager.generated.h"

class UAnimMontage;
class UBehaviorTree;
class UDataTable;
class UFloatingPawnMovement;
class UCapsuleComponent;

UCLASS()
class JH_CROPOUT_API AJH_Villager : public APawn , public IVillagersyInterface
{
	GENERATED_BODY()

public:

	AJH_Villager();
	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION()
	void Eat() const;

	/* IVillagersyInterface **/
	virtual void Action(AActor* Actor) override;
	virtual void ChangeJob(FName NewJob) override;
	/* ~IVillagersyInterface **/

	void ResetJobState();
	void StopJob();
	
protected:

	virtual void BeginPlay() override;
	

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UCapsuleComponent> Capsule;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Tool;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> Hat;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> Hair;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UDecalComponent> Decal;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UFloatingPawnMovement> FloatingPawnMovement;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	int32 Quantity;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	FName Job = "Idle";

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UDataTable> JobDataTable;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly , Category = "Job Profile")
	TObjectPtr<UBehaviorTree> ActiveBehavior;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly , Category = "Job Profile")
	TObjectPtr<AActor> TargetRef;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly , Category = "Job Profile")
	TObjectPtr<UAnimMontage> WorkAnim;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly , Category = "Job Profile")
	TObjectPtr<UStaticMesh> TargetTool;

private:
	FTimerHandle EatTimer;

	UFUNCTION()
	void AsyncLoadBehaviorTree();

	UFUNCTION()
	void AsyncLoadAnimMontage();

	UFUNCTION()
	void AsyncLoadSkeletalMesh();

	UFUNCTION()
	void AsyncLoadStaticMesh();
};
