// Fill out your copyright notice in the Description page of Project Settings.


#include "Villagers/JH_Villager.h"

#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameMode/ResourceInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameMode.h"

AJH_Villager::AJH_Villager()
{
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(Capsule);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);

	Tool = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tool"));
	Tool->SetupAttachment(SkeletalMesh);

	Hat = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hat"));
	Hat->SetupAttachment(SkeletalMesh);

	Hair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair"));
	Hair->SetupAttachment(SkeletalMesh);

	Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	Decal->SetupAttachment(RootComponent);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));

	
}

void AJH_Villager::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	//Set Random Skin tone and outfit tint
	SkeletalMesh->SetCustomPrimitiveDataFloat(0,FMath::FRand());
	SkeletalMesh->SetCustomPrimitiveDataFloat(1,FMath::FRand());
}

void AJH_Villager::BeginPlay()
{
	Super::BeginPlay();

	//Offset by capsule half height
	AddActorWorldOffset(FVector(0,0,Capsule->GetUnscaledCapsuleHalfHeight()));

	//Start Eat timer

	GetWorld()->GetTimerManager().SetTimer(EatTimer,this,&ThisClass::Eat,24.f,true);
}

void AJH_Villager::Eat() const
{
	if(IResourceInterface* Interface = Cast<IResourceInterface>( GetWorld()->GetAuthGameMode()))
	{
		Interface->RemoveTargetResource(EResourceType::Food,3);
	}
	
}
