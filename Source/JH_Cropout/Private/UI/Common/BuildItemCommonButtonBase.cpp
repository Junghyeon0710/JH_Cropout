// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Common/BuildItemCommonButtonBase.h"

#include "CommonTextBlock.h"
#include "Components/Border.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Engine/AssetManager.h"
#include "Interactable/Interactable.h"
#include "UI/Elements/CostWidget.h"

UBuildItemCommonButtonBase::UBuildItemCommonButtonBase()
{
	bApplyAlphaOnDisable = true;
	bLocked = false;
	bSelectable = false;
	bShouldSelectUponReceivingFocus = true;
	bInteractableWhenSelected = true;
	bToggleable = false;
	bTriggerClickedAfterSelection = false;
	bDisplayInputActionWhenNotInteractable = true;
	bHideInputActionWithKeyboard = false;
	bShouldUseFallbackDefaultInputAction = true;
	bRequiresHold = false;
	HoldData = nullptr;
	InputPriority = 0;
}

void UBuildItemCommonButtonBase::NativePreConstruct()
{
	Super::NativePreConstruct();

	//Set Title and Image
	Txt_Title->SetText(TableData.Title);
	Img_Icon->SetBrushFromSoftTexture(TableData.UIIcon);

	//Async Get Class
	FSoftObjectPath SoftObjectPath(TableData.TargetClass.ToSoftObjectPath());
	UAssetManager::GetStreamableManager().RequestAsyncLoad(SoftObjectPath, FStreamableDelegate::CreateWeakLambda(this,[this]()
	{
		HardClassRef = TableData.TargetClass.Get();
	}));

	BG->SetBrushColor(TableData.TabColor);

	//Set Resource Costs
	CostContainer->ClearChildren();

	TArray<EResourceType> Resources;
	TableData.Cost.GetKeys(Resources);
	for(const EResourceType Key : Resources)
	{
		if(CostWidgetClass)
		{
			UCostWidget* CostWidget = CreateWidget<UCostWidget>(this,CostWidgetClass);
			CostWidget->Cost = *TableData.Cost.Find(Key);
			CostWidget->Resource = Key;

			if(UHorizontalBoxSlot* HorizontalBoxSlot = CostContainer->AddChildToHorizontalBox(CostWidget))
			{
				FSlateChildSize Size;
				Size.Value = 1.f;
				Size.SizeRule = ESlateSizeRule::Fill;
				HorizontalBoxSlot->SetSize(Size);
			}
		}
	}
}

void UBuildItemCommonButtonBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	OnHovered().AddLambda([this]()
	{
		BaseSize->SetMinDesiredHeight(300.f);
		PlayAnimation(Loop_Hover,0,0);
		PlayAnimation(Highlight_In);
	});

	OnUnhovered().AddLambda([this]()
	{
		BaseSize->SetMinDesiredHeight(250.f);
		StopAnimation(Loop_Hover);
		PlayAnimation(Hightlight_Out);
	});
}
