// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TransitionWidget.h"

#include "Animation/WidgetAnimation.h"

void UTransitionWidget::TransIn()
{
	PlayAnimation(NewAnimation,0.f,1,EUMGSequencePlayMode::Forward,1.f);
}

void UTransitionWidget::TransOut()
{
	
	PlayAnimation(NewAnimation,0.f,1,EUMGSequencePlayMode::Reverse,1.f);

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindWeakLambda(this,[this]()
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,FString::Printf(TEXT("Remove")));
		RemoveFromParent();
	});
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,TimerDelegate,NewAnimation->GetEndTime(),false);
	
}
