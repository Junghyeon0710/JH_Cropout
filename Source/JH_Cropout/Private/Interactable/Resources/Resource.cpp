// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/Resources/Resource.h"

#include "Kismet/KismetArrayLibrary.h"

void AResource::BeginPlay()
{
	Super::BeginPlay();
}

void AResource::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	//Set the resource type to a tag so villagers can easily spot which resource is which without needing to cast
	Tags.Add(UEnum::GetValueAsName(ResourceType));

	//Pick Random mesh from collection
	if (!bUseRandomMesh || MeshList.IsEmpty()) return;
	
	Mesh->SetStaticMesh(MeshList[FMath::RandRange(0,MeshList.Num()-1)]);
	
}

void AResource::ScaleUp(float Delay)
{
	Mesh->SetHiddenInGame(true);

	FTimerHandle DelayTimer;
	GetWorld()->GetTimerManager().SetTimer(DelayTimer,FTimerDelegate::CreateLambda([this]()
	{
		Mesh->SetRelativeScale3D(FVector::ZeroVector);
		Mesh->SetHiddenInGame(false);
		StartAnimation();
	}),
	Delay,false);
}




