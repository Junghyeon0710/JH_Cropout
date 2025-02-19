// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VillagersyInterface.h"
#include "GameFramework/Pawn.h"
#include "GameMode/ResourceInterface.h"
#include "JH_Villager.generated.h"

class UAnimMontage;
class UBehaviorTree;
class UDataTable;
class UFloatingPawnMovement;
class UCapsuleComponent;

UCLASS()
class JH_CROPOUT_API AJH_Villager : public APawn , public IVillagersyInterface,public IResourceInterface
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
	virtual void ReturnToDefaultBT() override;
	virtual void PlayWorkAnim(float Delay) override;
	virtual float PlayDeliverAnim() override;
	/* ~IVillagersyInterface **/

	/* IResourceInterface **/
	virtual void AddResource(EResourceType Resource, int32 Value) override;
	virtual void RemoveResource(EResourceType& OutResource, int32& OutValue) override;
	/* ~IResourceInterface **/
	void ResetJobState();
	void StopJob();
	void PlayVillagerAnim(UAnimMontage* Montage,float Length);
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly , Category = "Job Profile")
	TObjectPtr<AActor> TargetRef;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly )
	TObjectPtr<UAnimMontage> VillagerMontage;

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
	EResourceType ResourcesHeld;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	int32 Quantity;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	FName Job = "Idle";

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UStaticMesh> CrateMesh;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UDataTable> JobDataTable;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly , Category = "Job Profile")
	TObjectPtr<UBehaviorTree> ActiveBehavior;

	
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
