// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Elements/BuildActivatableWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerInterface.h"

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

	if (IPlayerInterface* Interface = Cast<IPlayerInterface>(UGameplayStatics::GetGameMode(this)))
	{
		Interface->SwitchBuildMode(true);
	}
}
