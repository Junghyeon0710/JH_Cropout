// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

class UTextureRenderTarget2D;
UCLASS()
class JH_CROPOUT_API AInteractable : public AActor
{
	GENERATED_BODY()

public:
	AInteractable();
	void NextTickFunction();
protected:

	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	TObjectPtr<UTextureRenderTarget2D> RT_Draw;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	bool bEnableGroundBlend;
};
