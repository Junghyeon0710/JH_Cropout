// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JHBlueprintFunctionLibrary.generated.h"

class UJHGameInstance;
/**
 * 
 */
UCLASS()
class JH_CROPOUT_API UJHBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static UJHGameInstance* GetJhGameInstance(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure)
	static FVector SteppedPosition(const FVector& NewParam);
};
