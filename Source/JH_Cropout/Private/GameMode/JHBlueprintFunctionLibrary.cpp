// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/JHBlueprintFunctionLibrary.h"

#include "GameMode/JHGameInstance.h"
#include "Kismet/GameplayStatics.h"

UJHGameInstance* UJHBlueprintFunctionLibrary::GetJhGameInstance(const UObject* WorldContextObject)
{
	return Cast<UJHGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
}
