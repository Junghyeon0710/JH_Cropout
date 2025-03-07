// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResourceInterface.h"
#include "GameFramework/GameModeBase.h"
#include "Misc/IslandInterface.h"
#include "Player/PlayerInterface.h"
#include "ResourcesTypes/ResourcesTypes.h"
#include "JHGameMode.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FUpdateVillagers,int32 VillagerCount);
DECLARE_MULTICAST_DELEGATE_TwoParams(FUpdateResources,EResourceType Resource, int32 /*NewValue */ );

class ULayer_Game_ActivatableWidget;
class ASpawner;
class UTextureRenderTarget2D;
UCLASS()
class JH_CROPOUT_API AJHGameMode : public AGameModeBase , public IResourceInterface, public IPlayerInterface, public IIslandInterface
{
	GENERATED_BODY()
public:
	AJHGameMode();

	void GetSpawnRef();

	/** IResourceInterface */
	virtual void AddResource(EResourceType Resource, int32 Value) override;
	virtual bool CheckResource(EResourceType Resource, int32& OutValue) override;
	virtual void RemoveTargetResource(EResourceType Resource, int32 InValue) override;
	virtual TMap<EResourceType,int32> GetCurrentResources() override;
	/** ~IResourceInterface   */

	/** IPlayerInterface */
	virtual void AddUI(TSubclassOf<UCommonActivatableWidget> Widget) override;
	virtual void RemoveCurrentUILayer() override;
	/** ~IPlayerInterface   */

	/** IIslandInterface */
	virtual void IslandGenComplete() override;
	/** ~IIslandInterface */

	UFUNCTION()
	void LoadOrSpawnIslandAssets();

	void BeginAsyncSpawning();
	void SpawnVillagers(int32 AddCount);
	void SpawnVillager();

	void EndGame(bool bIsWin);
	
	FUpdateVillagers OnUpdateVillagers;
	FUpdateResources OnUpdateResources;
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

	UPROPERTY(EditAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	TMap<EResourceType,int32> Resources;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	TSoftClassPtr<AActor> TownHall_Ref;

	UPROPERTY(EditAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	TObjectPtr<AActor> TownHall;

	UPROPERTY(EditAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	TSubclassOf<APawn> VillagerClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	int32 VillagerCount = 0;
};
