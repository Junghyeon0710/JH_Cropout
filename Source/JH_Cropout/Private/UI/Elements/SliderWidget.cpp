// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Elements/SliderWidget.h"

#include "Components/TextBlock.h"
#include "GameMode/JHBlueprintFunctionLibrary.h"
#include "GameMode/JHGameInstance.h"
#include "Components/Slider.h"
#include "Kismet/GameplayStatics.h"


void USliderWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	//Set Initial Text
	MixDescriptor->SetText(SoundClassTitle);
}

void USliderWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Slider_67->OnValueChanged.AddDynamic(this,&ThisClass::SoundUpdate);
}

void USliderWidget::UpdateSlider() const
{
	//Get Stored Sound Class Volume in GI
	if (UJHGameInstance* GI = UJHBlueprintFunctionLibrary::GetJhGameInstance(this))
	{
		Slider_67->SetValue(GI->SoundMixes[Index]);
	}
}

void USliderWidget::SoundUpdate(float Value)
{
	//	Update Sound Class Volume
	if (UJHGameInstance* GI = UJHBlueprintFunctionLibrary::GetJhGameInstance(this))
	{
		GI->SoundMixes[Index] = Value;
		UGameplayStatics::SetSoundMixClassOverride(this,InSoundMixModifier,InSoundClass,Value);
	}
}



