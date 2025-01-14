// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/Interactable.h"

#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// #define TransformToTexture(InVec) \ 
// 	(GetActorLocation() + 100000.f) /20000.0f * InVec.X; \
// 	



AInteractable::AInteractable()
{

	PrimaryActorTick.bCanEverTick = true;

}

void AInteractable::BeginPlay()
{
	Super::BeginPlay();

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this; // 콜백이 발생할 객체
	LatentInfo.UUID = 1;             // 고유 ID
	LatentInfo.Linkage = 0;          // 내부 사용
	LatentInfo.ExecutionFunction = FName("NextTickFunction");
	UKismetSystemLibrary::DelayUntilNextTick(this, LatentInfo);
	
}

void AInteractable::NextTickFunction()
{
	if (!bEnableGroundBlend) return;

	check(RT_Draw);
	UCanvas* Canvas;
	FVector2D Size;
	FDrawToRenderTargetContext Context;
	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this,RT_Draw,Canvas,Size,Context);
	
}



