// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/MainMenuActivatableWidget.h"

#include "CommonButtonBase.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "GameMode/JHBlueprintFunctionLibrary.h"
#include "GameMode/JHGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Save/JHGameInstanceInterface.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "UI/Elements/PromptActivatableWidget.h"

void UMainMenuActivatableWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	//Reset focus on active (Get Desired Focus Target Is overriden function)
	if (IsValid(GetDesiredFocusTarget()))
	{
		GetDesiredFocusTarget()->SetFocus();
	};
	

	//Check if save exists
	if (UGameplayStatics::GetGameInstance(this)->Implements<UJHGameInstanceInterface>())
	{
		if (IJHGameInstanceInterface* Interface = Cast<IJHGameInstanceInterface>(UGameplayStatics::GetGameInstance(this)))
		{
			bHasSave = Interface->CheckSaveBool();
			BTN_Continue->SetIsEnabled(bHasSave); 
		}
	}

	//Set Donate to only visible on mobile
	const FString PlatformName = UGameplayStatics::GetPlatformName();
	if (PlatformName == "Android" || PlatformName == "iOS" )
	{
		BTN_Donate->SetVisibility(ESlateVisibility::Visible);
	}
	BTN_Donate->SetVisibility(ESlateVisibility::Collapsed);

	
	// Button Events Bind
	BTN_Continue->OnClicked().AddLambda([this]()
	{
		if (!Level.IsNull())
		{
			UJHBlueprintFunctionLibrary::GetJhGameInstance(this)->OpenLevel(Level);
		}
	});

	BTN_NewGame->OnClicked().AddLambda([this]()
	{
		if (bHasSave)
		{
			if (StackRef.IsValid())
			{
				const FString Question = FString::Printf(TEXT("Starting a new game will override your current save. Do you want to continue?"));
				UPromptActivatableWidget* PromptActivatableWidget = StackRef->AddWidget<UPromptActivatableWidget>(PromptWidgetClass);
				PromptActivatableWidget->PromptQuestion = FText::FromString(Question);
				PromptActivatableWidget->OnCallConfirm.BindUObject(this,&ThisClass::ConfirmNewGame);
			}
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

void UMainMenuActivatableWidget::ConfirmNewGame() const
{
	if (IJHGameInstanceInterface* Interface = Cast<IJHGameInstanceInterface>(UGameplayStatics::GetGameInstance(this)))
	{
		Interface->ClearSave(true);
	}
	
	if (!Level.IsNull())
	{
		UJHBlueprintFunctionLibrary::GetJhGameInstance(this)->OpenLevel(Level);
	}
}
