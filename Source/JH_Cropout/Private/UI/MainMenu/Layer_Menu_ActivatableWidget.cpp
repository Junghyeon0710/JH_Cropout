// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/Layer_Menu_ActivatableWidget.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "UI/MainMenu/MainMenuActivatableWidget.h"

void ULayer_Menu_ActivatableWidget::OnActivate()
{
	//Super::NativeOnActivated();
	
	MainStack->AddWidget<UMainMenuActivatableWidget>(ActivatableWidgetClass)->StackRef = MainStack;
	// BP_OnActivated();
	// OnActivated().Broadcast();
	//BP_OnWidgetActivated.Broadcast();
}

void ULayer_Menu_ActivatableWidget::NativeOnActivated()
{
	Super::NativeOnActivated();
	
	MainStack->AddWidget<UMainMenuActivatableWidget>(ActivatableWidgetClass)->StackRef = MainStack;
	
}

void ULayer_Menu_ActivatableWidget::AddStackItem(const TSubclassOf<UCommonActivatableWidget>& InActivatableWidgetClass)
{
	MainStack->AddWidget(InActivatableWidgetClass);
}
