// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Input/InputType.h"
#include "Kismet/KismetMathLibrary.h"

#include "BuildConfirmActivatableWidget.generated.h"

class AInteractable;
class UJHCommonButtonBase;
class UCommonBorder;
struct FVectorSpringState;
/**
 * 
 */
UCLASS()
class JH_CROPOUT_API UBuildConfirmActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:

	void UI_GameMain_AutoGenFunc(EInputType NewInput);
	void GetScreenLockedPosition(float& PositionX, float& PositionY);
	

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonBorder> CommonBorder_1;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UJHCommonButtonBase> BTN_Pos;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UJHCommonButtonBase> BTN_Pos_1;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UJHCommonButtonBase> BTN_Neg;

	
	
protected:
	virtual void NativeOnActivated() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY()
	TObjectPtr<AInteractable> taest;

	FVectorSpringState SpringState;
};
