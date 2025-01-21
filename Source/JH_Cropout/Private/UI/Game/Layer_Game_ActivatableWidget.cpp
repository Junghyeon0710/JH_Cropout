// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Game/Layer_Game_ActivatableWidget.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Elements/ResourceWidget.h"
#include "GameMode/JHGameMode.h"

void ULayer_Game_ActivatableWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//Get All Resources in Resource Enum and add widget to UI
	ResourceContainer->ClearChildren();

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindWeakLambda(this,[this,&TimerHandle]()
	{
		FString ResourceString = UEnum::GetValueAsString(Resources);
		if (ResourceString == "E_MAX" || ResourceString == "E MAX")
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
}
