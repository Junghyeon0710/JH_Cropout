// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/JHBlueprintFunctionLibrary.h"

#include "GameMode/JHGameInstance.h"
#include "GameMode/JHGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

UJHGameInstance* UJHBlueprintFunctionLibrary::GetJhGameInstance(const UObject* WorldContextObject)
{
	return Cast<UJHGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
}

AJHGameMode* UJHBlueprintFunctionLibrary::GetJhGameMode(const UObject* WorldContextObject)
{
	return Cast<AJHGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
}

FVector UJHBlueprintFunctionLibrary::SteppedPosition(const FVector& NewParam)
{
	const FVector DivideVector =  UKismetMathLibrary::Divide_VectorFloat(NewParam,200.f);
	
	constexpr float Value = 200.f; 
	return FVector(UKismetMathLibrary::Round(DivideVector.X) * Value,UKismetMathLibrary::Round(DivideVector.Y) * Value, 0.f );
}
