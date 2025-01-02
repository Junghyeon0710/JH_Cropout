// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/JHGameInstance.h"

#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "UI/TransitionWidget.h"
#include "Kismet/GameplayStatics.h"

void UJHGameInstance::Init()
{
	Super::Init();

	checkf(TransitionWidgetClass, TEXT("No TransitionWidgetClass"))
	UI_Transition = CreateWidget<UTransitionWidget>(GetWorld(),TransitionWidgetClass);
}

bool UJHGameInstance::CheckSaveBool()
{
	return bHasSave;
}

void UJHGameInstance::OpenLevel(const TSoftObjectPtr<UWorld>& Level) const
{
	if (IsValid(UI_Transition))
	{
		UI_Transition->TransIn();
		FTimerHandle Timer;
		FTimerDelegate  TimerDelegate;
		TimerDelegate.BindLambda([this,Level]()
		{
			UGameplayStatics::OpenLevelBySoftObjectPtr(this,Level);
		});
		GetWorld()->GetTimerManager().SetTimer(Timer,TimerDelegate,1.1f,false);
	}
	
	
}
