// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Input/InputType.h"
#include "BuildConfirmActivatableWidget.generated.h"


/**
 * 
 */
UCLASS()
class JH_CROPOUT_API UBuildConfirmActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:

	void UI_GameMain_AutoGenFunc(EInputType NewInput) const;
	void GetScreenLockedPosition(float& PositionX, float& PositionY);
protected:
	virtual void NativeOnActivated() override;


};
