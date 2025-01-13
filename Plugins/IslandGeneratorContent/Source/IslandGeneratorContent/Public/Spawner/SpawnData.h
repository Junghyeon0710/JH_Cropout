#pragma once

#include "SpawnData.generated.h"

USTRUCT(BlueprintType)
struct FSpawnData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<AActor> ClassRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BiomeScale;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 BiomeCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SpawnPerBiome;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RandomRotationRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ScaleRange;
};

