// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicMeshActor.h"
#include "IslandGen.generated.h"

/**
 * 
 */
UCLASS()
class ISLANDGENERATORCONTENT_API AIslandGen : public ADynamicMeshActor
{
	GENERATED_BODY()
public:
	AIslandGen();
	virtual void OnConstruction(const FTransform& Transform) override;

	static int32 PlatformSwitch(int32 LowEnd ,int32 HighEnd);
protected:
	virtual void BeginPlay() override;
	
private:

	void CreateIsland(bool SpawnMarkers);
	
	FRandomStream Seed;

	UPROPERTY()
	TObjectPtr<UDynamicMesh> DynamicMesh;

	TArray<FVector> SpawnPoints;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite , meta=(AllowPrivateAccess =true))
	int32 Islands{15};
	UPROPERTY(EditAnywhere,BlueprintReadWrite , meta=(AllowPrivateAccess =true))
	float Radius;
	UPROPERTY(EditAnywhere,BlueprintReadWrite , meta=(AllowPrivateAccess =true))
	FVector2D IslandsSize{800.f,5000.f};
	UPROPERTY(EditAnywhere,BlueprintReadWrite , meta=(AllowPrivateAccess =true))
	float MaxSpawnDistance;
	UPROPERTY(EditAnywhere,BlueprintReadWrite , meta=(AllowPrivateAccess =true))
	bool bPreConstruct;

	UPROPERTY(EditAnywhere,BlueprintReadWrite , meta=(AllowPrivateAccess =true))
	TSubclassOf<AActor> SpawnMarker;

	UPROPERTY(EditAnywhere,BlueprintReadWrite , meta=(AllowPrivateAccess =true))
	TObjectPtr<UMaterialParameterCollection> MaterialParameterCollection;
};
