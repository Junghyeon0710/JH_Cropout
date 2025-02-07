// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Elements/EndGameActivatableWidget.h"

#include "AudioModulationStatics.h"
#include "CommonTextBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameMode/JHBlueprintFunctionLibrary.h"
#include "GameMode/JHGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Common/JHCommonButtonBase.h"

void UEndGameActivatableWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	// 게임 인스턴스 캐싱 (중복 호출 방지)
	UJHGameInstance* GameInstance = UJHBlueprintFunctionLibrary::GetJhGameInstance(this);
	if (!GameInstance) return;

	// 버튼 클릭 이벤트 바인딩
	BTN_Continue->OnClicked().AddLambda([this]()
	{
		UGameplayStatics::SetGamePaused(this, false);
		DeactivateWidget();
	});

	BTN_MainMenu->OnClicked().AddLambda([GameInstance, this]()
	{
		GameInstance->OpenLevel(MainMenuLevel);
		UGameplayStatics::SetGamePaused(this, false);
	});

	BTN_Retry->OnClicked().AddLambda([GameInstance, this]()
	{
		GameInstance->ClearSave(false);
		GameInstance->OpenLevel(VillageLevel);
		UGameplayStatics::SetGamePaused(this, false);
	});
	
	// 플레이어 컨트롤러 캐싱
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController)
	{
		return;
	}

	// UI 입력 모드 설정
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController, GetDesiredFocusTarget());

	// 플레이어 입력 비활성화
	if (APawn* PlayerPawn = PlayerController->GetPawn())
	{
		PlayerPawn->DisableInput(PlayerController);
	}

	// 게임 일시 정지
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	
}

void UEndGameActivatableWidget::EndGame(bool InWin)
{
	bIsWin = InWin;

	if (bIsWin)
	{
		MainText->SetText(WinText);
		UAudioModulationStatics::SetGlobalBusMixValue(this,WinControlBus,1.0,0.f);
	}
	else
	{
		MainText->SetText(LoseText);
		UAudioModulationStatics::SetGlobalBusMixValue(this,LoseControlBus,0.f,0.f);
	}

	UJHBlueprintFunctionLibrary::GetJhGameInstance(this)->StopMusic();
}




