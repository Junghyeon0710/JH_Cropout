// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResourcesTypes/ResourcesTypes.h"
#include "ResourceWidget.generated.h"

/**
 * 
 */
UCLASS()
class JH_CROPOUT_API UResourceWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EResourceType ResourceType;
};
