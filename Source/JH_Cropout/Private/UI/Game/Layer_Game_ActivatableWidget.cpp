// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Game/Layer_Game_ActivatableWidget.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Elements/ResourceWidget.h"
#include "GameMode/JHGameMode.h"
#include "Player/JHPlayerController.h"
#include "UI/Common/JHCommonButtonBase.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "UI/Elements/EndGameActivatableWidget.h"
#include "UI/Elements/PauseActivatableWidget.h"

void ULayer_Game_ActivatableWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();


	BTN_Pause->OnClicked().AddLambda([this]()
	{
		MainStack->AddWidget<UPauseActivatableWidget>(PauseWidgetClass);
	});
	
	//Get All Resources in Resource Enum and add widget to UI
	ResourceContainer->ClearChildren();

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindWeakLambda(this,[this,&TimerHandle]()
	{
		FString ResourceString = UEnum::GetValueAsString(Resources);
		if (ResourceString == "EResourceType::EResourceType_MAX" || ResourceString == "EResourceType::EResourceType MAX")
		{
			GetWorld()->GetTimerManager().PauseTimer(TimerHandle);
		}
		else
		{
			checkf(ResourceWidgetClass, TEXT(" No ResourceWidgetClass"));
			UResourceWidget* ResourceWidget = CreateWidget<UResourceWidget>(GetWorld(),ResourceWidgetClass);
			ResourceWidget->ResourceType = Resources;
			ResourceContainer->AddChild(ResourceWidget);
			Resources = static_cast<EResourceType>(static_cast<int>(Resources) + 1);
		}
	});
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,TimerDelegate,.1f,true);

	//Bind to Villager Count
	AJHGameMode* GM = Cast<AJHGameMode>(UGameplayStatics::GetGameMode(this));
	GM->OnUpdateVillagers.BindWeakLambda(this,[this](int32 VillagerCount)
	{
		VillagerCounter->SetText(FText::AsNumber(VillagerCount));
	});

	Cast<AJHPlayerController>(UGameplayStatics::GetPlayerController(this,0))->OnCallKeySwitch.BindLambda([this](EInputType NewInput)
	{
		float Opacity = 0;
		if (NewInput != EInputType::Gamepad)
		{
			Opacity = 1;
		}

		BTN_Pause->SetRenderOpacity(Opacity);
	});
}

void ULayer_Game_ActivatableWidget::AddStackItem(const TSubclassOf<UCommonActivatableWidget>& UCommonActivatableWidgetClass) const
{
	MainStack->AddWidget<UCommonActivatableWidget>(UCommonActivatableWidgetClass);
}

void ULayer_Game_ActivatableWidget::PullCurrentActiveWidget() const
{
	MainStack->RemoveWidget(*MainStack->GetActiveWidget());
}

void ULayer_Game_ActivatableWidget::EndGame(bool Win)
{
	UEndGameActivatableWidget* EndGameWidget = MainStack->AddWidget<UEndGameActivatableWidget>(EndGameWidgetClass);

	EndGameWidget->ActivateWidget();
}
