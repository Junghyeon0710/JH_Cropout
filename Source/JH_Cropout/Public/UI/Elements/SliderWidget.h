// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SliderWidget.generated.h"

class USlider;
class UTextBlock;
/**
 * 
 */
UCLASS()
class JH_CROPOUT_API USliderWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateSlider() const;
protected:
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FText SoundClassTitle;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 Index;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<USoundClass> InSoundClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<USoundMix> InSoundMixModifier;

	UFUNCTION()
	void SoundUpdate(float Value);
private:
	UPROPERTY(meta=(BindWidget) ,EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	TObjectPtr<UTextBlock> MixDescriptor;

	UPROPERTY(meta=(BindWidget) ,EditAnywhere,BlueprintReadWrite, meta=(AllowPrivateAccess= ture))
	TObjectPtr<USlider> Slider_67;

	
	
};
