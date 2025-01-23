// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Misc/IslandInterface.h"

#include "JHMainMenuGM.generated.h"

class UMainMenuActivatableWidget;
class USoundControlBus;
class ULayer_Menu_ActivatableWidget;
/**
 * 
 */
class UCommonActivatableWidget;


UCLASS()
class JH_CROPOUT_API AJHMainMenuGM : public AGameModeBase ,public IIslandInterface
{
	GENERATED_BODY()
public:
	AJHMainMenuGM();
protected:

	//* IIslandInterface /
	virtual void SpawningComplete() override;
	//* ~ IIslandInterface/

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,Category = Widget)
	TSubclassOf<ULayer_Menu_ActivatableWidget> LayerMenuClass;

	UPROPERTY(EditAnywhere,Category = Widget)
	TObjectPtr<UCommonActivatableWidget> testMenuClass;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTextureRenderTarget2D> RenderTarget2D;	
	
	UFUNCTION()
	void OnLoginUISuccess(APlayerController* PlayerController);

	UPROPERTY(EditAnywhere,Category = Widget)
	TSubclassOf<UUserWidget> UserTest;

	UPROPERTY(EditAnywhere,Category = Widget)
	TObjectPtr<UUserWidget> ObjectTest;
	
private:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundControlBus> Cropout_Music_Piano_Vol;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundControlBus> Cropout_Music_Perc_Vol;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundControlBus> Cropout_Music_Strings_Delay;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundControlBus> Cropout_Music_WinLose;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> Audio;
};
