// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

class UBoxComponent;
class UMaterialInterface;

USTRUCT(BlueprintType)
struct FTransformTexture
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FVector2f ReturnValue;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FVector2f ReturnValue2;
	
};

class UTextureRenderTarget2D;
UCLASS()
class JH_CROPOUT_API AInteractable : public AActor
{
	GENERATED_BODY()

public:
	AInteractable();
	virtual void OnConstruction(const FTransform& Transform) override;
	
	UFUNCTION()
	void NextTickFunction();
	FTransformTexture TransformToTexture(FVector2d InVector);
protected:

	
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<USceneComponent> Scene;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UBoxComponent> Box;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TArray<UStaticMesh*> MeshList;
private: 
	UPROPERTY(EditAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	TObjectPtr<UTextureRenderTarget2D> RT_Draw;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	bool bEnableGroundBlend = true;

	UPROPERTY(EditAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	float ProgressionState;

	UPROPERTY(EditAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	float OutlineDraw;

	UPROPERTY(BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	float BoundGap;

	UPROPERTY(EditAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	TObjectPtr<UMaterialInterface> RenderMaterial;

	UPROPERTY(EditAnywhere,BlueprintReadOnly ,meta=(AllowPrivateAccess = true))
	TSubclassOf<AInteractable> OverlapnteractablClass;
public:
	FORCEINLINE float GetProgressionState() const {return ProgressionState;}
};
