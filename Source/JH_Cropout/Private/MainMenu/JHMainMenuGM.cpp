// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/JHMainMenuGM.h"
#include "CommonActivatableWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


AJHMainMenuGM::AJHMainMenuGM()
{
	GameSessionClass = nullptr;
	
}

void AJHMainMenuGM::BeginPlay()
{
	Super::BeginPlay();
	
	checkf(LayerMenuClass,TEXT("No LayerMenuClass"));

	
	UCommonActivatableWidget*  LayerMenu = CreateWidget<UCommonActivatableWidget>(UGameplayStatics::GetPlayerController(GetWorld(),0),LayerMenuClass);
	LayerMenu->AddToViewport();
	LayerMenu->ActivateWidget();
}
