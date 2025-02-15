// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Elements/BuildActivatableWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "GameFramework/Pawn.h"
#include "Interactable/Extras/ResourceStruct.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerInterface.h"
#include "UI/Common/JHCommonButtonBase.h"
#include "UI/Common/BuildItemCommonButtonBase.h"

void UBuildActivatableWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (!BuildItemClass) return;
	
	//Get our placeable actors from the Data Table DT_Placeable
	Container->ClearChildren();
	TArray<FName> RowNames = BuidablesDataTable->GetRowNames();
	
	for (auto Name : RowNames)
	{
		if (FResourceStruct* Resource = BuidablesDataTable->FindRow<FResourceStruct>(Name,TEXT("")))
		{
			//	//For each data table entry, make a new UI element and add it to the container
			if(!BuildItemClass) return;
			
			if (UBuildItemCommonButtonBase* Widget = CreateWidget<UBuildItemCommonButtonBase>(this,BuildItemClass))
			{
				Widget->TableData = *Resource;
				if (UHorizontalBoxSlot* HorizontalBoxSlot = Container->AddChildToHorizontalBox(Widget))
				{
					HorizontalBoxSlot->SetVerticalAlignment(VAlign_Bottom);
					FMargin Margin;
					Margin.Left = 8.f;
					Margin.Top = 0.f;
					Margin.Right = 8.f;
					Margin.Bottom = 5.f;
					
					HorizontalBoxSlot->SetPadding(Margin);
				}

			}
		}
	}
}

void UBuildActivatableWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	//Set Default Focus, Disable player input so background game doesn't move
	APlayerController* PC = UGameplayStatics::GetPlayerController(this,0);
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PC);

	GetDesiredFocusTarget()->SetFocus();

	APawn* Pawn = PC->GetPawn();

	Pawn->SetActorTickEnabled(false);
	Pawn->DisableInput(PC);

	if (IPlayerInterface* Interface = Cast<IPlayerInterface>(Pawn))
	{
		Interface->SwitchBuildMode(true);
	}

	BTN_Back->OnClicked().AddWeakLambda(this,[this]()
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(this,0))
		{
			if (APawn* Pawn = PC->GetPawn())
			{
				Pawn->EnableInput(PC);
				if (IPlayerInterface* Interface = Cast<IPlayerInterface>(Pawn))
				{
					Interface->SwitchBuildMode(false);
				}
				DeactivateWidget();
			}
		}
	});
}
