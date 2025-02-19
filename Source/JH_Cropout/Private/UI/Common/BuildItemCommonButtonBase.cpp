// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Common/BuildItemCommonButtonBase.h"

#include "CommonActivatableWidget.h"
#include "CommonTextBlock.h"
#include "Components/Border.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Engine/AssetManager.h"
#include "GameMode/JHGameMode.h"
#include "Interactable/Interactable.h"
#include "Kismet/GameplayStatics.h"
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

	// On pressed get player and trigger Begin Build, set focus back to game
	OnClicked().AddWeakLambda(this,[this]()
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(this,0))
		{
			if (IPlayerInterface* Interface = Cast<IPlayerInterface>(PC->GetPawn()))
			{
				Interface->BeginBuild(HardClassRef,TableData.Cost);
			}
			if (IPlayerInterface* Interface = Cast<IPlayerInterface>(GetWorld()->GetAuthGameMode()) ; BuildConfirmClass)
			{
				Interface->AddUI(BuildConfirmClass);
			}
		}
	});
	
}

void UBuildItemCommonButtonBase::NativeConstruct()
{
	Super::NativeConstruct();

	if (AJHGameMode* GM = GetWorld()->GetAuthGameMode<AJHGameMode>())
	{
		GM->OnUpdateResources.AddWeakLambda(this,[this](EResourceType Resource, int32 NewValue)
		{
			if (ResourceCheck())
			{
				SetIsInteractionEnabled(true);
			}
			else
			{
				SetIsInteractionEnabled(false);
			}
		});
	}
	if (ResourceCheck())
	{
		SetIsInteractionEnabled(true);
	}
	else
	{
		SetIsInteractionEnabled(false);
	}
}

bool UBuildItemCommonButtonBase::ResourceCheck()
{
	bEnableBuild = true;

	if (IResourceInterface* Interface = Cast<IResourceInterface>(GetWorld()->GetAuthGameMode()))
	{
		TArray<EResourceType> ResourceTypes;
		TableData.Cost.GetKeys(ResourceTypes);

		for (const EResourceType& Resource : ResourceTypes)
		{
			if (const int32* RequiredValue = TableData.Cost.Find(Resource))
			{
				int32 AvailableValue = 0;
				bool bCheckResource = Interface->CheckResource(Resource, AvailableValue);

				if (!bCheckResource || AvailableValue < *RequiredValue)
				{
					bEnableBuild = false;
					break;
				}
			}
		}
	}
    
	return bEnableBuild;
}
