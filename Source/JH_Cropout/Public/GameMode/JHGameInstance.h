// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Misc/IslandInterface.h"
#include "Save/JHGameInstanceInterface.h"
#include "JHGameInstance.generated.h"

class AInteractable;
class UTransitionWidget;
class UJHSaveGame;
class USoundControlBus;
class UAudioComponent;
/**
 * 
 */
UCLASS()
class JH_CROPOUT_API UJHGameInstance : public UGameInstance, public IJHGameInstanceInterface ,public IIslandInterface
{
	GENERATED_BODY()

public:

	void TransitionIn() const;
	void TransitionOut() const;
	void ShowTransitionWidget() const;
	
	/** IJHGameInstanceInterface */
	virtual bool CheckSaveBool() override;
	virtual void ClearSave(bool ClearSeed) override;
	virtual void SaveGame() override;
	virtual void UpdateAllInteractables() override;
	virtual void UpdateAllResources(TMap<EResourceType, int32> NewParam) override;
	virtual void UpdateAllVillagers() override;
	/** ~IJHGameInstanceInterface */

	/** IIslandInterface */
	virtual void SpawningComplete() override;
	virtual FRandomStream IslandSeed() override;
	virtual void ScaleUp(float Delay) override;
	/** ~IIslandInterface */
	
	void OpenLevel(const TSoftObjectPtr<UWorld>& Level) const;

	void PlayMusic(USoundBase* Audio,const float Volume,const bool Persist);
	void StopMusic();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<float> SoundMixes;
protected:

	virtual void Init() override;

	/**  Save */ 
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Save)
	bool bHasSave;

	FString SaveName{"SAVE"};

	UPROPERTY(BlueprintReadOnly,Category = Save)
	TObjectPtr<UJHSaveGame> SaveGameRef;

	// UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = Save)
	// TSubclassOf<UJHSaveGame> SaveGameClass;

	/**~  Save */ 
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TSubclassOf<UTransitionWidget> TransitionWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UTransitionWidget> UI_Transition;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<AInteractable> InteractableActorClass;


	bool bMusicPlaying;
private:
	void LoadGame();
	void SaveClear();
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundControlBus> Cropout_Music_WinLose;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundControlBus> Cropout_Music_Stop;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundControlBus> Cropout_Music_Piano;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAudioComponent> AudioComponent;
};
