// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/JHGameInstance.h"

#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "UI/TransitionWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Save/JHSaveGame.h"

void UJHGameInstance::Init()
{
	Super::Init();

	//Create Loading Screen Widget and store in GI
	checkf(TransitionWidgetClass, TEXT("No TransitionWidgetClass"));
	UI_Transition = CreateWidget<UTransitionWidget>(GetWorld(),TransitionWidgetClass);

	//Load Game
	LoadGame();
}

void UJHGameInstance::LoadGame()
{
	bHasSave = UGameplayStatics::DoesSaveGameExist(SaveName,0);
	if (bHasSave)
	{
		SaveGame = Cast<UJHSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveName,0));
	}
	else
	{
		SaveGame = Cast<UJHSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
		bMusicPlaying = false;
	}
}

void UJHGameInstance::TransitionIn() const
{
	checkf(TransitionWidgetClass, TEXT("No TransitionWidgetClass"));

	if (!UI_Transition->IsInViewport())
	{
		UI_Transition->AddToViewport();
	}
	
	UI_Transition->TransIn();
}

bool UJHGameInstance::CheckSaveBool()
{
	return bHasSave;
}

void UJHGameInstance::ClearSave(bool ClearSeed)
{
	SaveClear();
	if (ClearSeed)
	{
		const int64 RandStream = UKismetMathLibrary::RandomInteger64(2147483647);
		SaveGame->Seed = UKismetMathLibrary::MakeRandomStream(RandStream);
		bHasSave = false;
	}
}

void UJHGameInstance::SaveClear()
{
	check(SaveGame)
	SaveGame->PlayTime = 0.f;
	SaveGame->Villagers.Empty();
	SaveGame->Interactables.Empty();
	SaveGame->Resources.Empty();
	SaveGame->Resources.Add(EResourceType::Food,100);

	bMusicPlaying = false;
}

void UJHGameInstance::OpenLevel(const TSoftObjectPtr<UWorld>& Level) const
{
	if (IsValid(UI_Transition))
	{
		TransitionIn();
		FTimerHandle Timer;
		FTimerDelegate  TimerDelegate;
		TimerDelegate.BindLambda([this,Level]()
		{
			UGameplayStatics::OpenLevelBySoftObjectPtr(this,Level);
		});
		GetWorld()->GetTimerManager().SetTimer(Timer,TimerDelegate,1.1f,false);
	}
	
	
}
