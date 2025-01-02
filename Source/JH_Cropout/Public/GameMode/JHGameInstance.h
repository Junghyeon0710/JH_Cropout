// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Save/JHGameInstanceInterface.h"
#include "JHGameInstance.generated.h"

class UTransitionWidget;
/**
 * 
 */
UCLASS()
class JH_CROPOUT_API UJHGameInstance : public UGameInstance, public IJHGameInstanceInterface
{
	GENERATED_BODY()

public:
	/** IJHGameInstanceInterface */
	virtual bool CheckSaveBool() override;
	/** ~IJHGameInstanceInterface */

	
	void OpenLevel(const TSoftObjectPtr<UWorld>& Level) const;
protected:

	virtual void Init() override;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Save)
	bool bHasSave;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TSubclassOf<UTransitionWidget> TransitionWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UTransitionWidget> UI_Transition;
};
