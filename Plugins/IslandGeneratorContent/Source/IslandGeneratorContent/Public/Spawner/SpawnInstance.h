#pragma once

#include "SpawnInstance.generated.h"

USTRUCT(BlueprintType)
struct FSpawnInstance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> ClassRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BiomeScale;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 BiomeCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 SpawnPerBiome;

};