// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Elements/PromptActivatableWidget.h"
#include "UI/Common/JHCommonButtonBase.h"
#include "CommonTextBlock.h"

void UPromptActivatableWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//Event Dispatch, clear bindings and widget
	BTN_Pos->OnClicked().AddLambda([this]()
	{
		if (OnCallConfirm.IsBound())
		{
			OnCallConfirm.Execute();
		}
		ClearBindings();
	});

	BTN_Neg->OnClicked().AddLambda([this]()
	{
		if (OnCallBack.IsBound())
		{
			OnCallBack.Execute();
		}
		ClearBindings();
	});
}

void UPromptActivatableWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	if (IsValid(GetDesiredFocusTarget()))
	{
		GetDesiredFocusTarget()->SetFocus();
	}

	Title->SetText(PromptQuestion);

	
}

void UPromptActivatableWidget::ClearBindings()
{
	OnCallConfirm.Unbind();
	OnCallBack.Unbind();
	DeactivateWidget();
	
}
