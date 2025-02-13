// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/Interactable.h"

#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Canvas.h"

AInteractable::AInteractable()
{

	PrimaryActorTick.bCanEverTick = true;
	bEnableGroundBlend = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(RootComponent);
	
}

void AInteractable::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	
	// Creates a collision box around the bounds of the actor rounded to grid spacing. This is what is checked when placing actors to make sure they don't overlap.
	FVector Max;
	FVector Min;
	Mesh->GetLocalBounds(Min,Max);

	float Step = 100.f;

	FVector Value = Max / 100;
	float MaxValue = UKismetMathLibrary::Max(UKismetMathLibrary::Round(Value.X) * Step,UKismetMathLibrary::Round(Value.Y) * Step);

	FVector MakeVector = UKismetMathLibrary::MakeVector(
	UKismetMathLibrary::Max(MaxValue,100.f),
	UKismetMathLibrary::Max(MaxValue,100.f),
	UKismetMathLibrary::Max(UKismetMathLibrary::Round(Value.Z)*Step,100.f));
		
	Box->SetBoxExtent(MakeVector + BoundGap * 100.f);
	Box->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(FVector(1.f,0.f,0.f)));
	
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
	//Write position to world space RT, used to remove grass from area and paint dirt around asset.
	if (!bEnableGroundBlend) return;

	check(RT_Draw);
	UCanvas* Canvas;
	FVector2D Size;
	FDrawToRenderTargetContext Context;
	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this,RT_Draw,Canvas,Size,Context);
	
	FTransformTexture TransformTexture = TransformToTexture(Size);
	check(RenderMaterial);
	Canvas->K2_DrawMaterial(RenderMaterial,FVector2d(TransformTexture.ReturnValue),FVector2d(TransformTexture.ReturnValue2),FVector2d::ZeroVector);
	
	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, Context);

	//Remove if exactly overlapping another interactable. This will not be needed once Spawner is replaced by PCG.
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors,AInteractable::GetClass());

	for (auto&  Actor : Actors)
	{
		bool bIsOverlap = UKismetMathLibrary::EqualEqual_VectorVector(Actor->GetActorLocation() ,GetActorLocation(),5.f);
		bool bHasActorTag = Actor->ActorHasTag(FName("PlacementMode"));
		if (bIsOverlap && !bHasActorTag)
		{
			Destroy();
		}
	}
}

FTransformTexture AInteractable::TransformToTexture(FVector2d InVector)
{
	FTransformTexture OutTransformTexture;

	// 액터의 바운딩 박스 계산
	FVector Origin;
	FVector BoxExtent;
	GetActorBounds(false, Origin, BoxExtent);

	// X와 Y 중 최소값 계산
	const float MinExtent = UKismetMathLibrary::Min(BoxExtent.X, BoxExtent.Y);

	// 크기 변환 계산
	const float ScaleFactor = MinExtent / 10000.f;
	const float ScaledValue = InVector.X * ScaleFactor * OutlineDraw;

	// 위치 변환 계산
	const FVector AdjustedLocation = (GetActorLocation() + 10000.f) / 20000.f;
	const FVector2f TransformedLocation = FVector2f(FVector2d(AdjustedLocation * InVector.X - ScaledValue / 2));

	// 결과 구조체에 값 설정
	OutTransformTexture.ReturnValue = TransformedLocation;
	OutTransformTexture.ReturnValue2 = FVector2f(ScaledValue);

	return OutTransformTexture;
}

float AInteractable::Interact()
{
	return 0;
}

void AInteractable::SetProgressionState(float Progression)
{
	//Set progression state
	ProgressionState = Progression;

	//If this interactable requires building before use, apply the Build tag and set starting mesh from mesh list
	if(RequireBuild)
	{
		Tags.Add(FName("Build"));
		UStaticMesh* StaticMesh = MeshList[FMath::FloorToInt(ProgressionState)];
		if(IsValid(StaticMesh))
		{
			Mesh->SetStaticMesh(StaticMesh);
		}
	}
}

void AInteractable::PlacementMode()
{
	bEnableGroundBlend = false;
}



