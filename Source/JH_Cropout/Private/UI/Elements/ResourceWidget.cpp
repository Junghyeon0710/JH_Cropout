// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Elements/ResourceWidget.h"

#include "Animation/WidgetAnimation.h"
#include "Components/Image.h"
#include "GameMode/ResourceInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "GameMode/JHGameMode.h"

void UResourceWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	TSoftObjectPtr<UTexture2D> OutTexture;
	if (ResourceType == EResourceType::None)
	{
		OutTexture = NoneTexture;
	}
	else if (ResourceType == EResourceType::Food)
	{
		OutTexture = FoodTexture;
	}
	else if (ResourceType == EResourceType::Wood)
	{
		OutTexture = WoodTexture;
	}
	else if (ResourceType == EResourceType::Stone)
	{
		OutTexture = StoneTexture;
	}

	Image_24->SetBrushFromSoftTexture(OutTexture);
}

void UResourceWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(IResourceInterface* Interface = Cast<IResourceInterface>(UGameplayStatics::GetGameMode(this)))
	{
		Interface->CheckResource(ResourceType,Value);
	}
	
}

void UResourceWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Cast<AJHGameMode>(UGameplayStatics::GetGameMode(this))->OnUpdateResources.BindUObject(this,&ThisClass::UpdateValue);
	
}

void UResourceWidget::UpdateValue(EResourceType Resource, int32 NewValue)
{
	if (ResourceType != Resource)
	{
		return;
	}

	UWidgetAnimation* AnimationToPlay = (NewValue > this->Value) ? Increase : Reduce;
	PlayAnimation(AnimationToPlay);

	this->Value = NewValue;
}
