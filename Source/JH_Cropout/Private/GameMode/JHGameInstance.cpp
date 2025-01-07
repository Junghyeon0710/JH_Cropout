// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/JHGameInstance.h"

#include "AudioModulationStatics.h"
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
		SaveGameRef = Cast<UJHSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveName,0));
	}
	else
	{
		SaveGameRef = Cast<UJHSaveGame>(UGameplayStatics::CreateSaveGameObject(UJHSaveGame::StaticClass()));
		bMusicPlaying = false;
	}
}

void UJHGameInstance::SaveGame()
{
	FAsyncSaveGameToSlotDelegate SaveGameToSlotDelegate;
	SaveGameToSlotDelegate.BindWeakLambda(this,[this](const FString& SlotName, const int32 UserIndex, bool bWasSuccessful)
	{
		bHasSave = true;
	});
	UGameplayStatics::AsyncSaveGameToSlot(SaveGameRef,SaveName,0,SaveGameToSlotDelegate);
}

void UJHGameInstance::TransitionIn() const
{
	ShowTransitionWidget();
	
	UI_Transition->TransIn();
}

void UJHGameInstance::TransitionOut() const
{
	ShowTransitionWidget();

	UI_Transition->TransOut();
}

void UJHGameInstance::ShowTransitionWidget() const
{
	checkf(TransitionWidgetClass, TEXT("No TransitionWidgetClass"));

	if (!UI_Transition->IsInViewport())
	{
		UI_Transition->AddToViewport();
	}
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
		SaveGameRef->Seed = UKismetMathLibrary::MakeRandomStream(RandStream);
		bHasSave = false;
	}
}

void UJHGameInstance::SaveClear()
{
	check(SaveGameRef)
	SaveGameRef->PlayTime = 0.f;
	SaveGameRef->Villagers.Empty();
	SaveGameRef->Interactables.Empty();
	SaveGameRef->Resources.Empty();
	SaveGameRef->Resources.Add(EResourceType::Food,100);

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

void UJHGameInstance::PlayMusic(USoundBase* Audio, const float Volume, const bool Persist)
{
	if (bMusicPlaying) return;
	if (!IsValid(Cropout_Music_WinLose) && !IsValid(Cropout_Music_Stop) ) return;
	UAudioModulationStatics::SetGlobalBusMixValue(this,Cropout_Music_WinLose,0.5f,0.f);
	UAudioModulationStatics::SetGlobalBusMixValue(this,Cropout_Music_Stop,0.f,0.f);
	AudioComponent = UGameplayStatics::CreateSound2D(this,Audio,Volume,1.f,0.f,nullptr,Persist);

	AudioComponent->Play(0.f);
	bMusicPlaying = true;
}
