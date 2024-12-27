// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "JHMainMenuGM.generated.h"

/**
 * 
 */
class UCommonActivatableWidget;


UCLASS()
class JH_CROPOUT_API AJHMainMenuGM : public AGameModeBase
{
	GENERATED_BODY()
public:
	AJHMainMenuGM();
protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,Category = Widget)
	TSubclassOf<UCommonActivatableWidget> LayerMenuClass;

	

	

//	TObjectPtr<UCommonUserWidget> test;
	
	
private:

	
};
