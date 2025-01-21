// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "JHGameMode.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FUpdateVillagers,int32 VillagerCount);

class ULayer_Game_ActivatableWidget;
class ASpawner;
class UTextureRenderTarget2D;
UCLASS()
class JH_CROPOUT_API AJHGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AJHGameMode();

	void GetSpawnRef();


	FUpdateVillagers OnUpdateVillagers;
	
protected:
	virtual void BeginPlay() override;

private:

	float StartTime;

	UPROPERTY(EditAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	TObjectPtr<UTextureRenderTarget2D> RenderTarget2D;

	UPROPERTY(EditAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	TObjectPtr<ASpawner> SpawnerRef;

	UPROPERTY(EditAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	TSubclassOf<ULayer_Game_ActivatableWidget> Layer_Game_ActivatableWidgetClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	TObjectPtr<ULayer_Game_ActivatableWidget> UIHUD;
};
