#pragma once


#include "../../../../../Program Files/Epic Games/UE_5.4/Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/DataTable.h"
#include "Job.generated.h"

class UStaticMesh;
class USkeletalMesh;
class UAnimMontage;
class UBehaviorTree;

USTRUCT(BlueprintType)
struct FJob :public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UAnimMontage> WorkAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<USkeletalMesh> Hat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UStaticMesh> Tool;
	
};
