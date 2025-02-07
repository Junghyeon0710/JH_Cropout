// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Elements/PauseActivatableWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameMode/JHBlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Common/JHCommonButtonBase.h"
#include "UI/Elements/SliderWidget.h"
#include "GameMode/JHGameInstance.h"

void UPauseActivatableWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	// 게임 인스턴스 캐싱 (중복 호출 방지)
	UJHGameInstance* GameInstance = UJHBlueprintFunctionLibrary::GetJhGameInstance(this);
	if (!GameInstance) return;
	
	BTN_Resume->OnClicked().AddLambda([this]()
	{
		UGameplayStatics::SetGamePaused(this,false);
		UWidgetBlueprintLibrary::SetFocusToGameViewport();
		DeactivateWidget();
	});

	BTN_MainMenu->OnClicked().AddLambda([GameInstance, this]()
	{
		UGameplayStatics::SetGamePaused(this, false);
		GameInstance->OpenLevel(MainMenuLevel);
		
	});

	BTN_Restart->OnClicked().AddLambda([GameInstance, this]()
	{
		UGameplayStatics::SetGamePaused(this, false);
		GameInstance->ClearSave(false);
		GameInstance->OpenLevel(VillageLevel);
	});
	
	// GetDesiredFocusTarget 함수는 블프에서 구현해야 되는 BlueprintImplementableEvent함수임
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(UGameplayStatics::GetPlayerController(this,0),GetDesiredFocusTarget());

	GetDesiredFocusTarget()->SetFocus();
	UGameplayStatics::SetGamePaused(this, true);

	Slider_Music->UpdateSlider();
	Slider_SFX->UpdateSlider();
	
}
