// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/JHMainMenuGM.h"
#include "CommonActivatableWidget.h"
#include "Blueprint/UserWidget.h"


AJHMainMenuGM::AJHMainMenuGM()
{
	GameSessionClass = nullptr;
	
}

void AJHMainMenuGM::BeginPlay()
{
	Super::BeginPlay();

	UCommonActivatableWidget*  LayerMenu = CreateWidget<UCommonActivatableWidget>(GetWorld(),LayerMenuClass);
	LayerMenu->AddToViewport();
	LayerMenu->ActivateWidget();
}
