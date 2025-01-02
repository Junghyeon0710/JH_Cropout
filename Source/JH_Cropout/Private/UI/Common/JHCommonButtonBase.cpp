// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Common/JHCommonButtonBase.h"

#include "Components/SizeBox.h"
#include "CommonUI/Public/CommonTextBlock.h"
#include "CommonUI/Public/CommonActionWidget.h"
#include "Kismet/GameplayStatics.h"

void UJHCommonButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	
	ButtonTitle->SetText(ButtonText);
	


	FString PlatformName = UGameplayStatics::GetPlatformName();
	if (PlatformName == "Android" || PlatformName == "iOS")
	{
		ButtonSize->SetMinDesiredHeight(MinHeight *= 1.5);
	}
	else
	{
		ButtonSize->SetMinDesiredHeight(MinHeight);
	}
	

	GamepadIcon->SetInputAction(TriggeredInputAction);
}
