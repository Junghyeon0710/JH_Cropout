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

void UMainMenuActivatableWidget::OnActivate()
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
	//Continue Game
	BTN_Continue->OnClicked().AddLambda([this]()
	{
		if (!Level.IsNull())
		{
			UJHBlueprintFunctionLibrary::GetJhGameInstance(this)->OpenLevel(Level);
		}
	});

	//If save game already exists, prompt for new game
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
		else
		{
			ConfirmNewGame();
		}
	});

	// If save game already exists, prompt for new game
	BTN_Donate->OnClicked().AddLambda([this]()
	{
		if(bHasSave)
		{
			const FString Question = FString::Printf(TEXT("Would you like to donate £1.99 to help make the game better? "));
			UPromptActivatableWidget* PromptActivatableWidget = StackRef->AddWidget<UPromptActivatableWidget>(PromptWidgetClass);
			PromptActivatableWidget->PromptQuestion = FText::FromString(Question);
			PromptActivatableWidget->OnCallConfirm.BindUObject(this, &ThisClass::ConfirmDonate);
		}
		else
		{
			ConfirmNewGame();
		}
	});

	//Add Prompt and set question
	BTN_Quit->OnClicked().AddLambda([this]()
	{
		const FString Question = FString::Printf(TEXT("Are you sure you want to quit?"));
		UPromptActivatableWidget* PromptActivatableWidget = StackRef->AddWidget<UPromptActivatableWidget>(PromptWidgetClass);
		PromptActivatableWidget->PromptQuestion = FText::FromString(Question);
		PromptActivatableWidget->OnCallConfirm.BindUObject(this,&ThisClass::ConfirmQuit);
	});

	// ~Button Events Bind
}

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
	//Continue Game
	BTN_Continue->OnClicked().AddLambda([this]()
	{
		if (!Level.IsNull())
		{
			UJHBlueprintFunctionLibrary::GetJhGameInstance(this)->OpenLevel(Level);
		}
	});

	//If save game already exists, prompt for new game
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
		else
		{
			ConfirmNewGame();
		}
	});

	// If save game already exists, prompt for new game
	BTN_Donate->OnClicked().AddLambda([this]()
	{
		if(bHasSave)
		{
			const FString Question = FString::Printf(TEXT("Would you like to donate £1.99 to help make the game better? "));
			UPromptActivatableWidget* PromptActivatableWidget = StackRef->AddWidget<UPromptActivatableWidget>(PromptWidgetClass);
			PromptActivatableWidget->PromptQuestion = FText::FromString(Question);
			PromptActivatableWidget->OnCallConfirm.BindUObject(this, &ThisClass::ConfirmDonate);
		}
		else
		{
			ConfirmNewGame();
		}
	});

	//Add Prompt and set question
	BTN_Quit->OnClicked().AddLambda([this]()
	{
		const FString Question = FString::Printf(TEXT("Are you sure you want to quit?"));
		UPromptActivatableWidget* PromptActivatableWidget = StackRef->AddWidget<UPromptActivatableWidget>(PromptWidgetClass);
		PromptActivatableWidget->PromptQuestion = FText::FromString(Question);
		PromptActivatableWidget->OnCallConfirm.BindUObject(this,&ThisClass::ConfirmQuit);
	});

	// ~Button Events Bind
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

void UMainMenuActivatableWidget::ConfirmQuit() const
{
	UKismetSystemLibrary::QuitGame(this,nullptr,EQuitPreference::Quit,false);
}

void UMainMenuActivatableWidget::ConfirmDonate() const
{
	const FString Question = FString::Printf(TEXT("Waiting for update..."));
	UPromptActivatableWidget* PromptActivatableWidget = StackRef->AddWidget<UPromptActivatableWidget>(PromptWidgetClass);
	PromptActivatableWidget->PromptQuestion = FText::FromString(Question);
}

