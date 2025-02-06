// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Cheats.generated.h"


class UInputAction;
class UInputMappingContext;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) ,Blueprintable)
class JH_CROPOUT_API UCheats : public UActorComponent
{
	GENERATED_BODY()

public:	

	UCheats();

protected:
	virtual void BeginPlay() override;
	void Key1Event();
	void Key2Event();
	void Key3Event();

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> CheatInputMappingContext;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> Key1Action;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> Key2Action;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> Key3Action;
	
};

