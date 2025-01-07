// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/JHMenuPawn.h"

#include "Camera/CameraComponent.h"
#include "Camera/CameraShakeSourceComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Shakes/PerlinNoiseCameraShakePattern.h"

AJHMenuPawn::AJHMenuPawn()
{

	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	CameraShakeSource = CreateDefaultSubobject<UCameraShakeSourceComponent>(TEXT("CameraShakeSource"));
	CameraShakeSource->SetupAttachment(RootComponent);

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));

	SpringArm->SetRelativeRotation(FRotator(0.f,-30.f,0.f));
	SpringArm->TargetArmLength = 7800.f;
	SpringArm->SocketOffset = FVector(0.f,0.f,200.f);
	SpringArm->bDoCollisionTest = false;

	Camera->FieldOfView = 20.f;
	Camera->bLockToHmd = false;

	CameraShakeSource->Attenuation = ECameraShakeAttenuation::Linear;
	CameraShakeSource->InnerAttenuationRadius = 100000.0f;
	CameraShakeSource->OuterAttenuationRadius = 100000.0f;
	
	check(CameraShakeClass);
	UCameraShakeBase * CameraShakeBase = NewObject<UCameraShakeBase>(this);
	UPerlinNoiseCameraShakePattern* PerlinNoisePattern = NewObject<UPerlinNoiseCameraShakePattern>(this);
	CameraShakeBase->SetRootShakePattern(PerlinNoisePattern);
	CameraShakeSource->CameraShake = CameraShakeClass;
	CameraShakeSource->bAutoStart = true;

	RotatingMovement->RotationRate = FRotator(0.f,0.f,5.f);
	
}



