// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Elements/ResourceWidget.h"

#include "Components/Image.h"

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
