// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "BuildActivatableWidget.generated.h"

class UBuildItemCommonButtonBase;
class UJHCommonButtonBase;
class UHorizontalBox;
/**
 * 
 */
UCLASS()
class JH_CROPOUT_API UBuildActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeOnActivated() override;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	TObjectPtr<UHorizontalBox> Container;

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	TObjectPtr<UJHCommonButtonBase> BTN_Back;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	TObjectPtr<UDataTable> BuidablesDataTable;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	TSubclassOf<UBuildItemCommonButtonBase> BuildItemClass;
};
