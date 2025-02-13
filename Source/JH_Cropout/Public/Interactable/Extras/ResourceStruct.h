#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ResourcesTypes/ResourcesTypes.h"
#include "ResourceStruct.generated.h"

class UTexture2D;
class AInteractable;

USTRUCT(BlueprintType)
struct FResourceStruct :public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<AInteractable> TargetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> UIIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EResourceType,int32> Cost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FColor TabColor;
};
