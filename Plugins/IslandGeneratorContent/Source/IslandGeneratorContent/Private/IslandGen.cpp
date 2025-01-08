// Fill out your copyright notice in the Description page of Project Settings.


#include "IslandGen.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Misc/IslandInterface.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"


AIslandGen::AIslandGen()
{
	DynamicMeshComponent->PrimaryComponentTick.bCanEverTick = false;
}

void AIslandGen::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!bPreConstruct) return;

	CreateIsland(false);
}

void AIslandGen::BeginPlay()
{
	Super::BeginPlay();

	if (IIslandInterface* Interface = Cast<IIslandInterface>(UGameplayStatics::GetGameInstance(this)))
	{
		Seed = FRandomStream(Interface->IslandSeed().GetCurrentSeed());
	}
	CreateIsland(true);
}

void AIslandGen::CreateIsland(bool SpawnMarkers)
{
	//Set Ref and Clear Old Data
	DynamicMesh = DynamicMeshComponent->GetDynamicMesh();
	DynamicMesh->Reset();
	SpawnPoints.Empty();

	for (int32 Index = 0; Index<Islands ; ++Index )
	{
		//Append a bunch of cylinders together, this will form the basic Island Shape(실린더를 여러 개 추가하면 기본 아일랜드 모양이 됩니다)
		Radius = UKismetMathLibrary::RandomFloatInRangeFromStream(Seed,IslandsSize.X,IslandsSize.Y);
		const FVector Point = UKismetMathLibrary::Multiply_VectorVector(UKismetMathLibrary::RandomUnitVectorFromStream(Seed),FVector(MaxSpawnDistance / 2.f));
		SpawnPoints.Add(FVector(Point.X,Point.Y,0.f));
		FTransform SpawnTransform = UKismetMathLibrary::MakeTransform(FVector(SpawnPoints[Index].X,SpawnPoints[Index].Y,0.f),FRotator::ZeroRotator,FVector::ZeroVector);

		UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendCone(DynamicMesh,FGeometryScriptPrimitiveOptions(),SpawnTransform,Radius,Radius/4.f,1300.f,32,1.f);

		//Create markers for use with spawning (Useful Later on)
		if (SpawnMarkers)
		{
			GetWorld()->SpawnActor<AActor>(SpawnMarker,FTransform(SpawnPoints[Index]));
		}
	}
}
