// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnData.h"
#include "SpawnInstance.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

class ANavigationData;

UCLASS()
class ISLANDGENERATORCONTENT_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ASpawner();
	
	void AsyncLoadClasses();
	void AsyncLoadClass();
	UFUNCTION()
	void WaitForNavMeshAndAssets();
	void ReadyToSpawn();
	void SpawnAssets(TSubclassOf<AActor> Class ,const FSpawnData& SpawnParams );
	FVector SteppedPosition(const FVector& InParam) const;
	void SpawnInst(UInstancedStaticMeshComponent* Class, float Radius, int32 BiomeCount, int32 MaxPawn);
	void FinishSpawning() const;
protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	int32 ClassRefIndex;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	bool bAsyncComplete;

	UPROPERTY(EditAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	TArray<FSpawnData> SpawnTypes;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	TArray<FSpawnInstance> SpawnInstances;

	UPROPERTY(EditAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	bool bAutoSpawn;

	UPROPERTY(EditAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	int32 Counter;

	UPROPERTY(EditAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	float TotalCounter;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	int32 IndexCounter;

	UPROPERTY(EditAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	int32 MaxSpawn;

	FTimerHandle NavCheckHandle;

	UPROPERTY(EditAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	bool bActorSwitch;

	UPROPERTY(EditAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	bool bCallSave;

	UPROPERTY(EditAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	TObjectPtr<ANavigationData> NavData;

	FRandomStream Seed;
};
