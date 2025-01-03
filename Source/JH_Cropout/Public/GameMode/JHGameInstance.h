// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Save/JHGameInstanceInterface.h"
#include "JHGameInstance.generated.h"

class UTransitionWidget;
class UJHSaveGame;
/**
 * 
 */
UCLASS()
class JH_CROPOUT_API UJHGameInstance : public UGameInstance, public IJHGameInstanceInterface
{
	GENERATED_BODY()

public:

	void TransitionIn() const;
	
	/** IJHGameInstanceInterface */
	virtual bool CheckSaveBool() override;
	virtual void ClearSave(bool ClearSeed) override;
	/** ~IJHGameInstanceInterface */

	
	void OpenLevel(const TSoftObjectPtr<UWorld>& Level) const;
protected:

	virtual void Init() override;

	/**  Save */ 
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Save)
	bool bHasSave;

	FString SaveName{"SAVE"};

	UPROPERTY(BlueprintReadOnly,Category = Save)
	TObjectPtr<UJHSaveGame> SaveGame;

	UPROPERTY(BlueprintReadOnly,Category = Save)
	TSubclassOf<UJHSaveGame> SaveGameClass;

	/**~  Save */ 
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TSubclassOf<UTransitionWidget> TransitionWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UTransitionWidget> UI_Transition;


	bool bMusicPlaying;
private:
	void LoadGame();
	void SaveClear();

	
};
