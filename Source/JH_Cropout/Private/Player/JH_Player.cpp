// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/JH_Player.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Extras/Cheats.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Curves/CurveFloat.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Player/JHPlayerController.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetArrayLibrary.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "Components/BoxComponent.h"
#include "GameMode/JHBlueprintFunctionLibrary.h"
#include "GameMode/ResourceInterface.h"
#include "Villagers/VillagersyInterface.h"
#include "Interactable/Interactable.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Save/JHGameInstanceInterface.h"
#include "GameFramework/GameMode.h"

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

	Cast<AJHPlayerController>(NewController)->OnCallKeySwitch.AddUObject(this,&ThisClass::InputSwitch);

}

void AJH_Player::SwitchBuildMode(bool bIsInBuildMode)
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetPlayerController()->GetLocalPlayer()))
	{
		if (bIsInBuildMode)
		{
			Subsystem->RemoveMappingContext(VillagerMode);
			Subsystem->AddMappingContext(BuildMode,99);
		}
		else
		{
			Subsystem->RemoveMappingContext(BuildMode);
			Subsystem->AddMappingContext(VillagerMode,0);
		}
	}
}

void AJH_Player::BeginBuild(const TSubclassOf<AInteractable>& TargetClass,const TMap<EResourceType, int32>& InResourceCost)
{
	TargetSpawnClass = TargetClass;
	this->ResourceCost = InResourceCost;

	if(IsValid(Spawn))
	{
		Destroy();
	}
	Spawn = GetWorld()->SpawnActor<AInteractable>(TargetSpawnClass,FTransform(GetActorLocation()));
	Spawn->PlacementMode();
	CreateBuildOverlay();
}

void AJH_Player::BeginPlay()
{
	Super::BeginPlay();

	//On Begin Play start bound check
	UpdateZoom();

	FTimerHandle MoveRackingTimer;

	GetWorld()->GetTimerManager().SetTimer(MoveRackingTimer,FTimerDelegate::CreateWeakLambda(this,[this]()
	{
		MoveTracking();
	}),0.016667,true);

	//
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetPlayerController()->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(BaseInput, 0);
		Subsystem->AddMappingContext(VillagerMode, 0);
	}

	OnActorBeginOverlap.AddDynamic(this,&ThisClass::AJH_Player::BeginOverlap);
	OnActorEndOverlap.AddDynamic(this,&ThisClass::AJH_Player::EndOverlap);

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

	SpringArm->TargetArmLength =  FMath::Lerp(800.f,40000.0f,Alpha);
	SpringArm->SetRelativeRotation(FRotator(FMath::Lerp(-40.f,-55.f,Alpha),0.f,0.f));

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
	const auto [Direction, Strength] = EdgeMove();
	AddMovementInput(Direction, Strength);


	//Position Collision On Ground Plane Projection
	// const FVector CollisionWorldLocation = Collision->GetComponentToWorld().GetLocation();
	// FVector InterpVector =  FMath::VInterpTo(CollisionWorldLocation,
	// 	FVector(CollisionWorldLocation.X,CollisionWorldLocation.Y,-500)
	// 	,UGameplayStatics::GetWorldDeltaSeconds(this),
	// 	12.f );

	FVector2D ScreenPos;
	FVector Intersection;
	bool bIsInputPressed;
	ProjectMouseTouchToGroundPlane(ScreenPos,Intersection,bIsInputPressed);
	Intersection.Z +=10;
	// FVector TouchVector = Intersection;
	// if (!bIsInputPressed)
	// {
	// 	TouchVector = InterpVector;
	// }
	//
	// FVector CollisionPosition;
	// if (InputType == EInputType::Touch)
	// {
	// 	CollisionPosition = TouchVector;
	// }
	// else
	// {
	// 	InterpVector.Z += 10;
	// 	CollisionPosition = InterpVector;
	// }

	Collision->SetWorldLocation(Intersection);



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

void AJH_Player::InputSwitch(EInputType NewInput)
{
	InputType = NewInput;

	//Get new Input Type
	if(InputType == EInputType::KeyMouse)
	{
		Cursor->SetHiddenInGame(false);
	}
	else if(InputType == EInputType::Gamepad)
	{
		Collision->SetRelativeLocation(FVector(0.f,0.f,10.f));
	}
	else if(InputType == EInputType::Touch) //If Touch, we only want to use the cursor when it's being pressed
	{
		Cursor->SetHiddenInGame(true);
		Collision->SetRelativeLocation(FVector(0.f,0.f,-500.0f));
	}
	
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


	//  7. 평면과 레이 교차 검사
	float T;
	FVector Intersection;
	UKismetMathLibrary::LinePlaneIntersection(
		WorldPosition, 
		WorldPosition + WorldDirection * 100000.0f, 
		UKismetMathLibrary::MakePlaneFromPointAndNormal(FVector::ZeroVector, FVector(0, 0, 1)), 
		T, 
		Intersection
	);

	//  8. 터치 입력 확인 (Z 좌표 조정)
	float LX, LY;
	bool bTouchPressed;
	GetPlayerController()->GetInputTouchState(ETouchIndex::Touch1, LX, LY, bTouchPressed);
	float TouchValue = bTouchPressed ? 0.0f  : -500.0f;

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
		bReturnValue = bIsCurrentlyPressed;
	}
	else if (InputType == EInputType::KeyMouse)
	{
		bReturnValue = bMousePosition;
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

void AJH_Player::BeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (IsValid(HoverActor)) return;

	HoverActor = OtherActor;
	
	GetWorld()->GetTimerManager().SetTimer(
	ClosestHoverCheckTimer,
	this, 
	&ThisClass::ClosestHoverCheck, // 멤버 함수 바인딩
	0.01f, 
	true // 반복 실행
	);
}

void AJH_Player::ClosestHoverCheck()
{
	//End Hover Check if not overlapping anything
	TArray<AActor*> Actors;
	Collision->GetOverlappingActors(Actors,AActor::StaticClass());

	if (Actors.IsEmpty())
	{
		GetWorld()->GetTimerManager().PauseTimer(ClosestHoverCheckTimer);
		HoverActor = nullptr;
	}
	else
	{
		 AActor* NewHover = nullptr;
		// // Collision 영역과 겹치는 Villager (APawn)를 찾음 (Villager를 우선적으로 처리)
		// Collision->GetOverlappingActors(Actors, APawn::StaticClass());
		//
		// for (int i = 0; i < Actors.Num(); ++i)
		// {
		// 	// 첫 번째 액터를 NewHover로 설정
		// 	if (i == 0)
		// 	{
		// 		NewHover = Actors[i];
		// 		continue;
		// 	}
		//
		// 	// 현재 반복 중인 액터와 Collision 간의 거리 계산
		// 	float DistanceToCollision = UKismetMathLibrary::Vector_Distance(
		// 		Actors[i]->GetActorLocation(),
		// 		Collision->GetComponentLocation()
		// 	);
		//
		// 	// 현재 선택된 NewHover와 Collision 간의 거리 계산
		// 	float DistanceToNewHover = UKismetMathLibrary::Vector_Distance(
		// 		NewHover->GetActorLocation(),
		// 		Collision->GetComponentLocation()
		// 	);
		//
		// 	// 더 가까운 액터를 NewHover로 설정
		// 	if (DistanceToCollision < DistanceToNewHover)
		// 	{
		// 		NewHover = Actors[i];
		// 	}

		
		//}

		//Check distance of all overlapping actors and find closest one
		Collision->GetOverlappingActors(Actors,AActor::StaticClass());
		for (int i =0; i<Actors.Num(); ++i)
		{
			if (i == 0)
			{
				NewHover = Actors[i];
				continue;
			}
			float PawnToCollision = UKismetMathLibrary::Vector_Distance(Actors[i]->GetActorLocation(),Collision->GetComponentLocation());
			float CollisionToNewHover =  UKismetMathLibrary::Vector_Distance(Collision->GetComponentLocation(),NewHover->GetActorLocation());

			if (PawnToCollision < CollisionToNewHover)
			{
				NewHover = Actors[i];
			}
		}

		//If closest overlapping actor is the currently hovered asset, do nothing.
		if (HoverActor != NewHover)
		{
			HoverActor = NewHover;
		}

	}
}

void AJH_Player::EndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);

	if (Actors.IsEmpty())
	{
		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this; // 콜백이 발생할 객체
		LatentInfo.UUID = 1;             // 고유 ID
		LatentInfo.Linkage = 0;          // 내부 사용
		LatentInfo.ExecutionFunction = FName("WaitForHoverActorNullptr");
		UKismetSystemLibrary::DelayUntilNextTick(this,LatentInfo);
	}
}

void AJH_Player::WaitForHoverActorNullptr()
{
	HoverActor = nullptr;
}

void AJH_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// VillagerInputAction
		EnhancedInputComponent->BindAction(VillagerInputAction,ETriggerEvent::Triggered,this,&AJH_Player::VillagerActionTriggered);
		EnhancedInputComponent->BindAction(VillagerInputAction,ETriggerEvent::Started,this,&AJH_Player::VillagerActionStart);
		EnhancedInputComponent->BindAction(VillagerInputAction,ETriggerEvent::Canceled,this,&AJH_Player::VillagerActionCanceledAndCompleted);
		EnhancedInputComponent->BindAction(VillagerInputAction,ETriggerEvent::Completed,this,&AJH_Player::VillagerActionCanceledAndCompleted);

		// DragMove Action
		EnhancedInputComponent->BindAction(DragMoveAction,ETriggerEvent::Triggered,this,&AJH_Player::DragMoveTriggered);

		//Movement
		EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AJH_Player::MoveTriggered);
		EnhancedInputComponent->BindAction(ZoomAction,ETriggerEvent::Triggered,this,&AJH_Player::ZoomTriggered);
		EnhancedInputComponent->BindAction(SpinAction,ETriggerEvent::Triggered,this,&AJH_Player::SpinTriggered);

		//BulidMove
		EnhancedInputComponent->BindAction(BuildMoveAction,ETriggerEvent::Triggered,this,&AJH_Player::BuildMoveTriggered);
		EnhancedInputComponent->BindAction(BuildMoveAction,ETriggerEvent::Completed,this,&AJH_Player::BuildCompleted);

	}
}

void AJH_Player::VillagerActionTriggered()
{
	VillagerAction = HoverActor;
}

void AJH_Player::VillagerActionStart()
{
	//Check if multiple touch points and store current position if not.
	if(SingleTouchCheck())
	{
		PositionCheck();
		AActor* Actor;
		if(VillagerOverlapCheck(Actor))
		{
			VillagerSelect(Actor);
		}
		else
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetPlayerController()->GetLocalPlayer()))
			{
				checkf(DragMode,TEXT("No DragMode"));
				Subsystem->AddMappingContext(DragMode,1);
			}
		}
	}
}

void AJH_Player::VillagerActionCanceledAndCompleted()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetPlayerController()->GetLocalPlayer()))
	{
		checkf(DragMode,TEXT("No DragMode"));
		FModifyContextOptions Options;
		Options.bIgnoreAllPressedKeysUntilRelease = true;
		Options.bForceImmediately = true;
		Options.bNotifyUserSettings = false;
		Subsystem->RemoveMappingContext(DragMode,Options);
	}

	if(IsValid(Selected))
	{
		if(IVillagersyInterface* Interface = Cast<IVillagersyInterface>(Selected))
		{
			Interface->Action(VillagerAction);
			VillagerRelease();
		}
		VillagerAction = nullptr;
	}
}

void AJH_Player::DragMoveTriggered()
{
	if(SingleTouchCheck())
	{
		TrackMove();
	}
	else
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetPlayerController()->GetLocalPlayer()))
		{
			checkf(DragMode,TEXT("No DragMode"));
			FModifyContextOptions Options;
			Options.bIgnoreAllPressedKeysUntilRelease = true;
			Options.bForceImmediately = true;
			Options.bNotifyUserSettings = false;
			Subsystem->RemoveMappingContext(DragMode,Options);
		}
	}
}

void AJH_Player::MoveTriggered(const FInputActionValue& Value)
{
	// Movement and Rotation using WASD,QE or Controller
	const FVector2D Values = Value.Get<FVector2D>();

	AddMovementInput(GetActorForwardVector(),Values.Y);
	AddMovementInput(GetActorRightVector(),Values.X);
	
}

void AJH_Player::ZoomTriggered(const FInputActionValue& Value)
{
	const float ActionValue = Value.Get<float>();

	ZoomDirection = ActionValue;

	UpdateZoom();
	Dof();
}

void AJH_Player::SpinTriggered(const FInputActionValue& Value)
{
	const float ActionValue = Value.Get<float>();

	AddActorLocalRotation(FRotator(0.f,ActionValue,0.f));
}

void AJH_Player::BuildMoveTriggered()
{
	//GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,TEXT("Click"));
	UpdateBuildAsset();
}

void AJH_Player::BuildCompleted()
{
	if(IsValid(Spawn))
	{
		SetActorLocation(UJHBlueprintFunctionLibrary::SteppedPosition(GetActorLocation()));
	}
}

bool AJH_Player::SingleTouchCheck() const
{
	float LocationX;
	float LocationY;
	bool bIsCurrentlyPressed;
	GetPlayerController()->GetInputTouchState(ETouchIndex::Touch2,LocationX,LocationY,bIsCurrentlyPressed);

	return !bIsCurrentlyPressed;
}

void AJH_Player::PositionCheck()
{
	FVector2D ScreenPos;
	FVector Intersection;
	bool bReturnValue;
	ProjectMouseTouchToGroundPlane(ScreenPos,Intersection,bReturnValue);
	TargetHandle = Intersection;

	if(InputType != EInputType::Touch)
	{
		Collision->SetWorldLocation(TargetHandle);
	}

}

bool AJH_Player::VillagerOverlapCheck(AActor*& OverlapActor) const
{
	TArray<AActor*> OverlapActors;
	GetOverlappingActors(OverlapActors,APawn::StaticClass());

	if(OverlapActors.Num() > 0)
	{
		OverlapActor = OverlapActors[0];
		return true;
	}
	
	OverlapActor = nullptr;
	return false;
}


void AJH_Player::VillagerSelect(AActor* InSelected)
{
	Selected = InSelected;

	NSPath = UNiagaraFunctionLibrary::SpawnSystemAttached(
		NS_Target,
		RootComponent,
		TEXT("None"),
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget,
		false);

	
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUFunction(this,FName("UpdatePath"));
	GetWorld()->GetTimerManager().SetTimer(UpdatePathTimer,TimerDelegate,0.01f,true);
}

void AJH_Player::VillagerRelease()
{
	GetWorld()->GetTimerManager().PauseTimer(UpdatePathTimer);
	NSPath->DestroyComponent();
	Selected = nullptr;
}

void AJH_Player::UpdatePath()
{
	// NavPath: 현재 위치(Collision)에서 목표 위치(Selected)까지의 경로를 동기적으로 계산
	const UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
		this,  // 현재 액터 (AJH_Player)
		Collision->GetComponentLocation(),  // 출발 지점: Collision 컴포넌트의 위치
		Selected->GetActorLocation()  // 목표 지점: Selected 액터의 위치
	);

	// 경로 계산 결과가 유효한지 확인
	if (NavPath->PathPoints.Num() > 0)
	{
		// PathPoints 배열을 경로로 업데이트
		PathPoints = NavPath->PathPoints;

		// 출발점과 목표점을 명시적으로 설정
		PathPoints[0] = Collision->GetComponentLocation();  // 첫 번째 포인트: Collision의 위치
		PathPoints[PathPoints.Num() - 1] = Selected->GetActorLocation();  // 마지막 포인트: Selected의 위치

		// 경로 정보를 Niagara 시스템에 전달
		UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(NSPath, FName("TargetPath"), PathPoints);
	}
}

void AJH_Player::TrackMove()
{
	// 카메라와 스프링 암 끝 사이의 오프셋을 고려한 카메라 위치 계산

	const FVector SocketOffset = SpringArm->SocketOffset;

	// SpringArm의 위치와 카메라의 오프셋을 계산
	const FVector SpringArmLocation = SpringArm->GetComponentLocation();
	const FVector ForwardOffset = SpringArm->GetForwardVector() * (SpringArm->TargetArmLength - SocketOffset.X) * -1.f;
	const FVector UpwardOffset = SpringArm->GetUpVector() * SocketOffset.Z;

	// 최종 카메라 위치 (앞쪽 및 위쪽 오프셋을 포함)
	const FVector TargetCameraPosition = SpringArmLocation + ForwardOffset + UpwardOffset;

	// 현재 카메라 위치와 목표 카메라 위치 간의 차이를 계산 (오프셋)
	const FVector CameraOffset = TargetCameraPosition - Camera->GetComponentLocation();

	// 마우스나 터치 입력을 월드 평면에 투영
	FVector2D MouseScreenPos;
	FVector MouseWorldIntersection;
	bool bIsValidInput;
	ProjectMouseTouchToGroundPlane(MouseScreenPos, MouseWorldIntersection, bIsValidInput);

	if (bIsValidInput)
	{
		// 목표 위치를 계산하고 캐릭터의 월드 위치를 업데이트
		FVector MoveDelta = TargetHandle - MouseWorldIntersection - CameraOffset;
		
		AddActorWorldOffset(FVector(MoveDelta.X, MoveDelta.Y, 0)); // Z축 이동은 제외
	}
}

void AJH_Player::CreateBuildOverlay()
{
	if(!IsValid(SpawnOverlay))
	{
		FVector Origin;
		FVector BoxExtent;
		Spawn->GetActorBounds(false,Origin,BoxExtent);
		if (UStaticMeshComponent* StaticMeshComponent = AddStaticMeshComponent(FTransform(FRotator::ZeroRotator,FVector::ZeroVector,FVector(BoxExtent/50.f))))
		{
			SpawnOverlay = StaticMeshComponent;
			FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld,EAttachmentRule::KeepWorld,true);
			SpawnOverlay->AttachToComponent(Spawn->Mesh,Rules);
			UpdateBuildAsset();
		}
	}
}

void AJH_Player::SpawnBuildTarget()
{
	if(!bCanDrop)
	{
		return;
	}

	if(AInteractable* SpawnActor = GetWorld()->SpawnActor<AInteractable>(TargetSpawnClass,Spawn->GetTransform()))
	{
		SpawnActor->SetProgressionState(0.f);
		RemoveResources();
		if(IJHGameInstanceInterface* Interface = Cast<IJHGameInstanceInterface>( GetGameInstance()))
		{
			Interface->UpdateAllInteractables();
		}
		UpdateBuildAsset();
	}
}

void AJH_Player::RotateSpawn()
{
	Spawn->SetActorRotation(UKismetMathLibrary::ComposeRotators(Spawn->GetActorRotation(),FRotator(0,0,90)));
}

void AJH_Player::DestroySpawn()
{
	Spawn->Destroy();
	SpawnOverlay->DestroyComponent();
}

void AJH_Player::RemoveResources()
{
	//Remove Resources
	TArray<EResourceType> Resources;
	ResourceCost.GetKeys(Resources);

	for(auto Resource : Resources)
	{
		if(IResourceInterface* Interface = Cast<IResourceInterface>(GetWorld()->GetAuthGameMode()))
		{
			Interface->RemoveTargetResource(Resource,*ResourceCost.Find(Resource));
		}
	}

	//Check if we have enough resources to place again
	
	for(auto Resource : Resources)
	{
		if(IResourceInterface* Interface = Cast<IResourceInterface>(GetWorld()->GetAuthGameMode()))
		{
			if(Interface->GetCurrentResources().Find(Resource) <= ResourceCost.Find(Resource))
			{
				if(IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(GetWorld()->GetAuthGameMode()))
				{
					PlayerInterface->RemoveCurrentUILayer();
					DestroySpawn();
					break;
				}
			}
		}
	}
}

void AJH_Player::UpdateBuildAsset()
{
	//Update Target Spawn Position
	FVector2D ScreenPos;
	FVector Intersection;
	bool bReturn;
	ProjectMouseTouchToGroundPlane(ScreenPos,Intersection,bReturn);

	if(!bReturn || !IsValid(Spawn))
	{
		return;
	}

	FVector InterpVector = FMath::VInterpTo(Spawn->GetActorLocation(),Intersection,UGameplayStatics::GetWorldDeltaSeconds(this),18.f);

	Spawn->SetActorLocation(InterpVector);

	//If not overlapping anything and within bounds, asset can be spawned
	TArray<AActor*> OverlayActors;
	Spawn->GetOverlappingActors(OverlayActors,AInteractable::StaticClass());
	
	if(OverlayActors.Num() == 0)
	{
		bCanDrop = CornersInNav();
	}
	else
	{
		bCanDrop = false;
	}

	UKismetMaterialLibrary::SetVectorParameterValue(
		this,
		MPC_Cropout,
		FName("Target Position"),
		FLinearColor(InterpVector.X,InterpVector.Y,InterpVector.Z,bCanDrop));
}

// 주어진 점들을 기준으로 Line Trace를 실행하여 모든 라인에 대해 충돌이 있는지 체크하는 함수
bool AJH_Player::CornersInNav()
{
    // Spawn 객체의 Box 컴포넌트의 Bounds 정보를 가져옴
    FVector Origin, BoxExtent;
    float SphereRadius;
    UKismetSystemLibrary::GetComponentBounds(Spawn->Box, Origin, BoxExtent, SphereRadius);

    // Box의 X, Y 크기를 1.05배로 확대하여 트레이스를 위한 4개의 모서리 위치 계산
    float HalfWidth = BoxExtent.X * 1.05;
    float HalfHeight = BoxExtent.Y * 1.05;
    float NegHalfWidth = HalfWidth * -1;
    float NegHalfHeight = HalfHeight * -1;

    // 4개의 모서리 점 계산
    FVector TopRight = Origin + FVector(HalfWidth, HalfHeight, 0);
    FVector TopLeft = Origin + FVector(NegHalfWidth, HalfHeight, 0);
    FVector BottomRight = Origin + FVector(HalfWidth, NegHalfHeight, 0);
    FVector BottomLeft = Origin + FVector(NegHalfWidth, NegHalfHeight, 0);

    // 트레이스를 무시할 액터 목록
    TArray<FHitResult> OutHits;
    TArray<AActor*> IgnoreActors;
    IgnoreActors.Add(Spawn);

    // 4개의 코너에서 LineTrace를 실행하여 충돌을 확인
    if (!PerformLineTrace(TopRight, IgnoreActors, OutHits)) return false;
    if (!PerformLineTrace(TopLeft, IgnoreActors, OutHits)) return false;
    if (!PerformLineTrace(BottomRight, IgnoreActors, OutHits)) return false;
    if (!PerformLineTrace(BottomLeft, IgnoreActors, OutHits)) return false;

    // 네 개의 라인 모두에서 충돌이 없으면 true 반환
    return true;
}

// 주어진 위치에서 LineTrace를 실행하고 결과를 반환하는 함수
bool AJH_Player::PerformLineTrace(const FVector& StartPoint, const TArray<AActor*>& IgnoreActors, TArray<FHitResult>& OutHits)
{
    // LineTrace를 실행하여 주어진 점에서 아래로 충돌을 확인
    return UKismetSystemLibrary::LineTraceMultiByProfile(
        this,
        FVector(StartPoint.X, StartPoint.Y, 100),  // Z 값은 100으로 고정하여 위에서 아래로 트레이스
        FVector(StartPoint.X, StartPoint.Y, -1.f),  // -1로 Z 값을 낮추어 트레이스 진행
        "Visibility",  // 프로파일 이름은 "Visibility"로 설정
        false,          // 트레이스에서 충돌이 발생한 오브젝트는 무시하지 않음
        IgnoreActors,   // 트레이스에서 무시할 액터 리스트
        EDrawDebugTrace::ForOneFrame,  // 디버그 표시: 1 프레임 동안만 표시
        OutHits,        // 충돌 결과가 저장될 배열
        true            // 히트 결과를 캐스팅하여 처리
    );
}
