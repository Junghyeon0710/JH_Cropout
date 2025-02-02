// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/JH_Player.h"

#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Extras/Cheats.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Curves/CurveFloat.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"


AJH_Player::AJH_Player()
{
 
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Root);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Cursor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cursor"));
	Cursor->SetupAttachment(Root);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetupAttachment(Root);

	Cheats = CreateDefaultSubobject<UCheats>(TEXT("Cheats"));

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
}

void AJH_Player::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	
}

void AJH_Player::BeginPlay()
{
	Super::BeginPlay();

	//On Begin Play start bound check
	UpdateZoom();
	
}

void AJH_Player::UpdateZoom()
{
	// //ZoomValue를 0~1로 조정
	// ZoomCurve를 이용해 부드럽게 보간
	// 카메라 거리(TargetArmLength)와 회전 변경
	// 이동 속도(MaxSpeed) 변경
	// DOF 효과 적용 (Dof() 함수)  Depth of Field, 심도 효과
	// 시야각(FOV) 변경
	checkf(ZoomCurve,TEXT("No ZoomCurve"));
	ZoomDirection *= 0.01;

	ZoomValue = FMath::Clamp(ZoomDirection + ZoomValue , 0.f,1.f);

	const float Alpha = ZoomCurve->GetFloatValue(ZoomValue);

	SpringArm->TargetArmLength =  FMath::Lerp(800.f,40000.f,Alpha);
	SpringArm->SetRelativeRotation(FRotator(0.f,FMath::Lerp(-40.f,-55.f,Alpha),0.f));

	FloatingPawnMovement->MaxSpeed =  FMath::Lerp(1000.f,6000.f,Alpha);
	Dof();

	Camera->SetFieldOfView(FMath::Lerp(20.f,15.f,Alpha)); 
}

void AJH_Player::Dof()
{
	PostProcessSettings.DepthOfFieldFstop = 3.f;
	PostProcessSettings.DepthOfFieldSensorWidth = 150.f;
	PostProcessSettings.DepthOfFieldFocalDistance = SpringArm->TargetArmLength;
	
}

void AJH_Player::MoveTracking()
{
	//Keep player within playspace
	FVector Location = GetActorLocation();
	UKismetMathLibrary::Vector_Normalize(Location,0.0001);
	Location.Z = 0;
	const float Size = FMath::Max((GetActorLocation().Size() - 9000.f) /5000.f,0);
	AddMovementInput(Location*-1.f,Size);

	//Syncs 3D Cursor and Collision Position
	UpdateCursorPosition();

	//Edge Of Screen Movement
	
}

void AJH_Player::UpdateCursorPosition()
{
	if(InputType == EInputType::Unknown || InputType == EInputType::Touch)
	{
		 return;
	}

	//Set Target to Hover Actor if valid, otherwise use Collision component.
	FTransform Target;
	if(IsValid(HoverActor))
	{
		FVector HoverOrigin;
		FVector HoverBoxExtent;
		HoverActor->GetActorBounds(true, HoverOrigin, HoverBoxExtent);

		// HoverOrigin의 Z 값을 고정
		HoverOrigin.Z = 20.f;

		// 2D 크기 계산
		const FVector2D HoverBoxExtent2D = FVector2D(HoverBoxExtent.X, HoverBoxExtent.Y);
		const float MaxDimension = UKismetMathLibrary::GetAbsMax2D(HoverBoxExtent2D) / 50.f;

		// 스케일 인디케이터 계산 (상하 변동)
		const float ScaleOscillation = UKismetMathLibrary::Sin(UKismetSystemLibrary::GetGameTimeInSeconds(this) * 5.f) * 0.25f + 1.f;

		// 최종 스케일 계산
		const float ScaleFactor = MaxDimension + ScaleOscillation;

		// FTransform 생성
		const FVector Scale(ScaleFactor, ScaleFactor, 1.f);
		Target = FTransform(FRotator(), HoverOrigin, Scale);
	}
	else
	{
		Target = Collision->GetComponentToWorld();
		Target.SetScale3D(FVector(2.f,2.f,1.f));
	}

	//Interp Cursor Position to target

	const FTransform InterTransform = UKismetMathLibrary::TInterpTo(
		Cursor->GetComponentToWorld(),Target,UGameplayStatics::GetWorldDeltaSeconds(this),12.f);
	
	Cursor->SetWorldTransform(InterTransform);

	
}

const FEdgeMoveVector& AJH_Player::EdgeMove()
{
	FEdgeMoveVector MoveVector;
	
	return MoveVector;
}

void AJH_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

