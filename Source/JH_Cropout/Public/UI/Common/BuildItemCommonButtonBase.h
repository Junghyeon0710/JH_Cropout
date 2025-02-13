// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "Interactable/Extras/ResourceStruct.h"
#include "BuildItemCommonButtonBase.generated.h"

class UCostWidget;
class USizeBox;
class UHorizontalBox;
class UCommonTextBlock;
class UBorder;
class UImage;
class AInteractable;
/**
 * 
 */
UCLASS()
class JH_CROPOUT_API UBuildItemCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()
public:
	UBuildItemCommonButtonBase();

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	TObjectPtr<USizeBox> BaseSize;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	TObjectPtr<UImage> Img_Icon;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	TObjectPtr<UBorder> BG;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	TObjectPtr<UCommonTextBlock> Txt_Title;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	TObjectPtr<UHorizontalBox> CostContainer;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FResourceStruct TableData;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FText ButtonText;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSoftObjectPtr<AInteractable> SpawnClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<AInteractable> HardClassRef;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UCostWidget> CostWidgetClass;
	
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;

	UPROPERTY(Transient,meta=(BindWidgetAnim),BlueprintReadOnly)
	TObjectPtr<UWidgetAnimation> Loop_Hover;

	UPROPERTY(Transient,meta=(BindWidgetAnim),BlueprintReadOnly)
	TObjectPtr<UWidgetAnimation> Hightlight_Out;

	UPROPERTY(Transient,meta=(BindWidgetAnim),BlueprintReadOnly)
	TObjectPtr<UWidgetAnimation> Highlight_In;
};
