// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/MainMenuActivatableWidget.h"

#include "CommonButtonBase.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "GameMode/JHBlueprintFunctionLibrary.h"
#include "GameMode/JHGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Save/JHGameInstanceInterface.h"

void UMainMenuActivatableWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	//Reset focus on active (Get Desired Focus Target Is overriden function)
	if (UWidget* FocusWidget = GetDesiredFocusTarget())
	{
		GetDesiredFocusTarget()->SetFocus();
	};
	

	//Check if save exists
	if (UGameplayStatics::GetGameInstance(this)->Implements<UJHGameInstanceInterface>())
	{
		if (IJHGameInstanceInterface* Interface = Cast<IJHGameInstanceInterface>(UGameplayStatics::GetGameInstance(this)))
		{
			BTN_Continue->SetIsEnabled(Interface->CheckSaveBool()); 
		}
	}

	//Set Donate to only visible on mobile
	const FString PlatformName = UGameplayStatics::GetPlatformName();
	if (PlatformName == "Android" || PlatformName == "iOS" )
	{
		BTN_Donate->SetVisibility(ESlateVisibility::Visible);
	}
	BTN_Donate->SetVisibility(ESlateVisibility::Collapsed);

	//
	
	BTN_Continue->OnClicked().AddLambda([this]()
	{
		if (Level.IsValid())
		{
			UJHBlueprintFunctionLibrary::GetJhGameInstance(this)->OpenLevel(Level);
		}
	});

	BTN_NewGame->OnClicked().AddLambda([this]()
	{
		if (Level.IsValid())
		{
			UJHBlueprintFunctionLibrary::GetJhGameInstance(this)->OpenLevel(Level);
		}
	});

	BTN_Donate->OnClicked().AddLambda([]()
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Black,FString::Printf(TEXT("Button Click")));
	});

	BTN_Quit->OnClicked().AddLambda([]()
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Black,FString::Printf(TEXT("Button Click")));
	});
}
