// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Elements/CostWidget.h"

#include "CommonTextBlock.h"
#include "Components/Image.h"

void UCostWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	C_Cost->SetText(FText::AsNumber(Cost));

	UTexture2D* Texture = nullptr;
	if(Resource == EResourceType::Food && FoodTexture)
	{
		Texture = FoodTexture;
	}
	else if(Resource == EResourceType::Food && StoneTexture)
	{
		Texture = StoneTexture;
	}
	else if(Resource == EResourceType::Food && WoodTexture)
	{
		Texture = WoodTexture;
	}
	
	if(Texture)
	{
		Image_17->SetBrushFromTexture(Texture);
	}

}
