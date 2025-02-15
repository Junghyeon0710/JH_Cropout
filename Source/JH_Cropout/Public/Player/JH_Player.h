// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerInterface.h"
#include "GameFramework/Pawn.h"
#include "Input/InputType.h"
#include "JH_Player.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
class UInputAction;
class UInputMappingContext;
class UCheats;
class UFloatingPawnMovement;
class USphereComponent;
class UCameraComponent;
class USpringArmComponent;
class USceneComponent;
class UCurveFloat;
struct FInputActionValue;

USTRUCT(BlueprintType)
struct FEdgeMoveVector
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FVector Direction;

	UPROPERTY(BlueprintReadOnly)
	float Strength;
	
};

UCLASS()
class JH_CROPOUT_API AJH_Player : public APawn, public IPlayerInterface
{
	GENERATED_BODY()

public:

	AJH_Player();
	virtual void PossessedBy(AController* NewController) override;

	/* IPlayerInterface **/
	virtual void SwitchBuildMode(bool bIsInBuildMode) override;
	virtual void BeginBuild(const TSubclassOf<AInteractable>& TargetClass, const TMap<EResourceType, int32>& InResourceCost) override;
	/* ~IPlayerInterface **/
	
	void UpdateZoom();
	void Dof();
	void MoveTracking();
	void UpdateCursorPosition();
	void InputSwitch(EInputType NewInput);
	void UpdateBuildAsset();
	FEdgeMoveVector EdgeMove();
	UFUNCTION(BlueprintImplementableEvent)
	UStaticMeshComponent* AddStaticMeshComponent(const FTransform& Transform);
	
	/** 
	 * 화면에서 입력한 위치를 월드 공간의 평면으로 변환하는 기능
	 * @param OutScreenPos	선택된 입력 좌표 (스크린 좌표)
	 * @param OutIntersection 평면과의 교차점 (월드 좌표)
	 * @param OutReturnValue	입력이 유효한지 여부 반환
	 */
	void ProjectMouseTouchToGroundPlane(FVector2D& OutScreenPos, FVector& OutIntersection,bool& OutReturnValue);
	
	APlayerController* GetPlayerController() const;
	void CursorDistFromViewportCenter(const FVector2D& CursorPosition,FVector& OutDirection, float& OutStrength);

	UFUNCTION()
	void BeginOverlap( AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void EndOverlap(AActor* OverlappedActor, AActor* OtherActor );

	FTimerHandle ClosestHoverCheckTimer;
	void ClosestHoverCheck();

	void WaitForHoverActorNullptr();
	void CreateBuildOverlay();

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<AInteractable> Spawn;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TSubclassOf<AInteractable> TargetSpawnClass;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TMap<EResourceType, int32> ResourceCost;
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> Cursor;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<USphereComponent> Collision;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<UCheats> Cheats;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<UFloatingPawnMovement> FloatingPawnMovement;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TObjectPtr<UCurveFloat> ZoomCurve;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> NS_Target;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TObjectPtr<UNiagaraComponent> NSPath;
	
	UPROPERTY(BlueprintReadOnly)
	FPostProcessSettings PostProcessSettings;

	UPROPERTY(BlueprintReadOnly)
	EInputType InputType;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<AActor> HoverActor;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<AActor> Selected;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> SpawnOverlay;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float EdgeMoveDistance = 50.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector TargetHandle;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TObjectPtr<AActor> VillagerAction;
	
	/* Input*/
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite , Category = Input)
	TObjectPtr<UInputMappingContext> BaseInput;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite , Category = Input)
	TObjectPtr<UInputMappingContext> VillagerMode;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite , Category = Input)
	TObjectPtr<UInputMappingContext> DragMode;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite , Category = Input)
	TObjectPtr<UInputMappingContext> BuildMode;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite , Category = Input)
	TObjectPtr<UInputAction> VillagerInputAction;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite , Category = Input)
	TObjectPtr<UInputAction> DragMoveAction;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite , Category = Input)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite , Category = Input)
	TObjectPtr<UInputAction> ZoomAction;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite , Category = Input)
	TObjectPtr<UInputAction> SpinAction;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TObjectPtr<UInputAction> BuildMoveAction;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite , Category = Input)
	TObjectPtr<UMaterialParameterCollection> MPC_Cropout;

	/* ~Input*/
	
	/* Input Binding Function*/
	void VillagerActionTriggered();
	void VillagerActionStart();
	void VillagerActionCanceledAndCompleted();

	//void VillagerActionCompleted();
	void DragMoveTriggered();

	//Movement
	void MoveTriggered(const FInputActionValue& Value);
	void ZoomTriggered(const FInputActionValue& Value);
	void SpinTriggered(const FInputActionValue& Value);

	//BuildMode
	void BuildMoveTriggered();
	void BuildCompleted();
public:	


	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	void PositionCheck();
	void VillagerSelect(AActor* InSelected);
	void VillagerRelease();
	bool CornersInNav();
	bool PerformLineTrace(const FVector& StartPoint, const TArray<AActor*>& IgnoreActors, TArray<FHitResult>& OutHits);
	
	UFUNCTION()
	void UpdatePath();

	void TrackMove();
	
	
private:

	bool SingleTouchCheck() const;
	bool VillagerOverlapCheck(AActor*& OverlapActor) const;
	
	float ZoomDirection = 0.f;
	float ZoomValue = .5f;
	TArray<FVector> PathPoints;

	FTimerHandle UpdatePathTimer;

	FVector StoredMove;

	bool bCanDrop;
};
