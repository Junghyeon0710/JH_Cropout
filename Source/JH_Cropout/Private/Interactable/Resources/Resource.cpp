// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/Resources/Resource.h"

#include "Kismet/KismetArrayLibrary.h"

void AResource::Death()
{
	//What to do when the resource is to be destroyed 
	Destroy();
}

float AResource::Interact()
{
	return CollectionTime;
}

void AResource::BeginPlay()
{
	Super::BeginPlay();
}

void AResource::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	//Set the resource type to a tag so villagers can easily spot which resource is which without needing to cast
	const FString FullEnumName = UEnum::GetValueAsString(ResourceType);
	FString EnumName;
	FullEnumName.Split(TEXT("::"), nullptr, &EnumName);
	Tags.Add(FName(EnumName));

	//Pick Random mesh from collection
	if (!bUseRandomMesh || MeshList.IsEmpty()) return;
	
	Mesh->SetStaticMesh(MeshList[FMath::RandRange(0,MeshList.Num()-1)]);
	
}

void AResource::ScaleUp(float Delay)
{
	Mesh->SetHiddenInGame(true);

	FTimerHandle DelayTimer;
	GetWorld()->GetTimerManager().SetTimer(DelayTimer,FTimerDelegate::CreateWeakLambda(this,[this]()
	{
		Mesh->SetRelativeScale3D(FVector::ZeroVector);
		Mesh->SetHiddenInGame(false);
		StartAnimation();
	}),
	Delay,false);

	
}

void AResource::RemoveResource(EResourceType& OutResource, int32& OutValue)
{
	EndWobble();
	OutResource = ResourceType;
	OutValue = CollectionValue;
	if (ResourceAmount == -1)
	{
		return;
	}

	ResourceAmount = FMath::Max( ResourceAmount - CollectionValue, 0);

	if(ResourceAmount <= 0 )
	{
		Death();
	}
}




