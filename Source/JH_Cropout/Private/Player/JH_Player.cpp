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
	
	//Edge Of Screen Movement
	AddMovementInput(EdgeMove().Direction,EdgeMove().Strength);

	//Position Collision On Ground Plane Projection
	const FVector CollisionWorldLocation = Collision->GetComponentToWorld().GetLocation();
	FVector InterpVector =  FMath::VInterpTo(CollisionWorldLocation,
		FVector(CollisionWorldLocation.X,CollisionWorldLocation.Y,-500)
		,UGameplayStatics::GetWorldDeltaSeconds(this),
		12.f );

	FVector2D ScreenPos;
	FVector Intersection;
	bool bIsInputPressed;
	ProjectMouseTouchToGroundPlane(ScreenPos,Intersection,bIsInputPressed);

	FVector TouchVector = Intersection;
	if (!bIsInputPressed)
	{
		TouchVector = InterpVector;
	}

	FVector CollisionPosition;
	if (InputType == EInputType::Touch)
	{
		CollisionPosition = TouchVector;
	}
	else
	{
		InterpVector.Z += 10;
		CollisionPosition = InterpVector;
	}

	Collision->SetWorldLocation(CollisionPosition);

	//Syncs 3D Cursor and Collision Position
	UpdateCursorPosition();
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

FEdgeMoveVector AJH_Player::EdgeMove()
{
	FEdgeMoveVector OutEdgeMoveVector;

	// 뷰포트 크기 가져오기 (중앙 좌표 계산)
	int32 SizeX, SizeY;
	GetPlayerController()->GetViewportSize(SizeX, SizeY);
	FVector2D ViewPortSize(SizeX / 2, SizeY / 2); // 화면 중앙 좌표

	// 현재 입력(마우스 또는 터치) 위치를 게임 월드 평면으로 투영
	FVector2D ScreenPos;
	FVector Intersection;
	bool bReturnValue;
	ProjectMouseTouchToGroundPlane(ScreenPos, Intersection, bReturnValue);

	// 커서(입력 위치)와 화면 중앙 거리 계산
	FVector Direction;
	float Strength;
	CursorDistFromViewportCenter(ScreenPos - ViewPortSize, Direction, Strength);

	// 방향 벡터를 월드 좌표계로 변환
	FVector OutDirection = UKismetMathLibrary::TransformDirection(GetActorTransform(), Direction);

	// 결과 값 설정
	OutEdgeMoveVector.Direction = OutDirection;  // 최종 이동 방향
	OutEdgeMoveVector.Strength = Strength;       // 이동 강도
	
	return OutEdgeMoveVector;
}

void AJH_Player::ProjectMouseTouchToGroundPlane(FVector2D& OutScreenPos, FVector& OutIntersection, bool& OutReturnValue)
{
	//  1. 뷰포트 크기 가져오기 (화면 중앙 좌표 계산)
	int32 SizeX;
	int32 SizeY;
	GetPlayerController()->GetViewportSize(SizeX, SizeY);
	FVector2D ViewPortSize = FVector2D(SizeX, SizeY) / 2; // 화면 중앙 좌표

	//  2. 마우스 위치 가져오기
	float LocationX, LocationY;
	bool bMousePosition = GetPlayerController()->GetMousePosition(LocationX, LocationY);
	FVector2D MouseSize = FVector2D(LocationX, LocationY); // 마우스 위치

	//  3. 터치 입력 상태 가져오기
	float TouchLocationX, TouchLocationY;
	bool bIsCurrentlyPressed;
	GetPlayerController()->GetInputTouchState(ETouchIndex::Touch1, TouchLocationX, TouchLocationY, bIsCurrentlyPressed);
	FVector2D TouchSize = FVector2D(TouchLocationX, TouchLocationY); // 터치 위치

	//  4. 입력 타입에 따라 선택할 위치 결정
	FVector2D GamePadSize = ViewPortSize / 2; // 게임패드 기본 위치 (화면 중앙)
	FVector2D KeyMouseSize = bMousePosition ? MouseSize : GamePadSize; // 마우스 입력이 없으면 기본값 사용
	FVector2D Touch = bIsCurrentlyPressed ? TouchSize : GamePadSize; // 터치 입력이 없으면 기본값 사용

	//  5. 현재 입력 타입(`InputType`)에 따라 입력 좌표 결정
	FVector2D SelectInputPosition;
	if (InputType == EInputType::Unknown || InputType == EInputType::Gamepad)
	{
		SelectInputPosition = GamePadSize;
	}
	else if (InputType == EInputType::KeyMouse)
	{
		SelectInputPosition = KeyMouseSize;
	}
	else if (InputType == EInputType::Touch)
	{
		SelectInputPosition = Touch;
	}

	//  6. 스크린 좌표를 월드 좌표로 변환 (Deproject)
	FVector WorldPosition, WorldDirection;
	UGameplayStatics::DeprojectScreenToWorld(GetPlayerController(), SelectInputPosition, WorldPosition, WorldDirection);

	// Debug: 월드 방향 벡터 출력
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("WorldDirection: %s"), *WorldDirection.ToString()));

	//  7. 평면과 레이 교차 검사
	float T;
	FVector Intersection;
	UKismetMathLibrary::LinePlaneIntersection(
		WorldPosition, 
		WorldPosition + WorldDirection * 100000.0f, 
		UKismetMathLibrary::MakePlaneFromPointAndNormal(FVector(), FVector(0, 0, 1)), 
		T, 
		Intersection
	);

	//  8. 터치 입력 확인 (Z 좌표 조정)
	float LX, LY;
	bool bTouchPressed;
	GetPlayerController()->GetInputTouchState(ETouchIndex::Touch1, LX, LY, bTouchPressed);
	float TouchValue = bTouchPressed ? -500.0f : 0.0f;

	// 터치 입력이면 Z 좌표 조정
	if (InputType == EInputType::Touch)
	{
		Intersection.Z += TouchValue;
	}

	//  9. 반환할 값 결정
	bool bReturnValue = false;
	if (InputType == EInputType::Unknown)
	{
		bReturnValue = false;
	}
	else if (InputType == EInputType::Gamepad)
	{
		bReturnValue = true;
	}
	else if (InputType == EInputType::Touch)
	{
		bReturnValue = bMousePosition;
	}
	else if (InputType == EInputType::KeyMouse)
	{
		bReturnValue = bIsCurrentlyPressed;
	}

	//  10. 결과 값 설정 (출력)
	OutScreenPos = SelectInputPosition;  // 선택된 입력 좌표 (스크린 좌표)
	OutIntersection = Intersection;      // 평면과의 교차점 (월드 좌표)
	OutReturnValue = bReturnValue;       // 입력이 유효한지 여부 반환
}


APlayerController* AJH_Player::GetPlayerController() const
{
	return Cast<APlayerController>(GetController());
}

void AJH_Player::CursorDistFromViewportCenter(const FVector2D& CursorPosition,FVector& OutDirection, float& OutStrength)
{
	// 뷰포트 크기 가져오기 (화면 절반 크기로 계산)
	int32 SizeX, SizeY;
	GetPlayerController()->GetViewportSize(SizeX, SizeY);
	float HalfSizeX = SizeX / 2.0f;
	float HalfSizeY = SizeY / 2.0f;
	
	float InputMultiplier  = 0;
	if (InputType == EInputType::Gamepad || InputType == EInputType::Touch)
	{
		InputMultiplier = 2.f;
	}
	else if (InputType == EInputType::KeyMouse)
	{
		InputMultiplier = 1.f;
	}
	
	HalfSizeX -= EdgeMoveDistance * InputMultiplier;
	HalfSizeY -= EdgeMoveDistance * InputMultiplier;
	
	// 커서 위치 보정 (화면 중앙 기준으로 오프셋 적용)
	FVector2D AdjustedCursorPos = CursorPosition;
	AdjustedCursorPos.X = UKismetMathLibrary::Abs(CursorPosition.X) - HalfSizeX;
	AdjustedCursorPos.Y = UKismetMathLibrary::Abs(CursorPosition.Y) - HalfSizeY;

	// X, Y 좌표가 EdgeMoveDistance 범위를 초과한 값만 남기고 나머지는 0으로 설정
	float MaxSizeX = FMath::Max(AdjustedCursorPos.X, 0) / EdgeMoveDistance;
	float MaxSizeY = FMath::Max(AdjustedCursorPos.Y, 0) / EdgeMoveDistance;

	// X, Y 부호를 계산하여 방향 벡터 구성
	FVector Direction = FVector(
		UKismetMathLibrary::SignOfFloat(CursorPosition.Y) * MaxSizeY * -1,  // 위/아래 방향 (음수 적용)
		UKismetMathLibrary::SignOfFloat(CursorPosition.X) * MaxSizeX,       // 좌/우 방향
		0
	);

	// 결과 값 설정
	OutDirection = Direction;
	OutStrength = 1.f;
}

void AJH_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

