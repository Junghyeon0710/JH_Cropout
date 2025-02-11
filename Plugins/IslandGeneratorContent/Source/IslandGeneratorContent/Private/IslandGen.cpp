// Fill out your copyright notice in the Description page of Project Settings.


#include "IslandGen.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Misc/IslandInterface.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"
#include "GeometryScript/MeshVoxelFunctions.h"
#include "GeometryScript/MeshNormalsFunctions.h"
#include "GeometryScript/MeshDeformFunctions.h"
#include "GeometryScript/MeshSubdivideFunctions.h"
#include "GeometryScript/MeshBooleanFunctions.h"
#include "GeometryScript/MeshUVFunctions.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "GameFramework/GameMode.h"


AIslandGen::AIslandGen()
{
	DynamicMeshComponent->PrimaryComponentTick.bCanEverTick = false;
}

void AIslandGen::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (bPreConstruct)
	{
		CreateIsland(false);
	}
	else
	{
		DynamicMeshComponent->GetDynamicMesh()->Reset();
	}
	
	
}

int32 AIslandGen::PlatformSwitch(int32 LowEnd, int32 HighEnd)
{
	const FString PlatformName = UGameplayStatics::GetPlatformName();
	if (PlatformName == "Android" || PlatformName == "Switch" || PlatformName == "IOS")
	{
		return LowEnd;
	}
	return HighEnd;
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

	for (int32 Index = 0; Index<=Islands ; ++Index )
	{
		//Append a bunch of cylinders together, this will form the basic Island Shape(실린더를 여러 개 추가하면 기본 아일랜드 모양이 됩니다)
		Radius = UKismetMathLibrary::RandomFloatInRangeFromStream(Seed,IslandsSize.X,IslandsSize.Y);
		const FVector Point = UKismetMathLibrary::Multiply_VectorVector(UKismetMathLibrary::RandomUnitVectorFromStream(Seed),FVector(MaxSpawnDistance / 2.f));
		SpawnPoints.Add(FVector(Point.X,Point.Y,0.f));
		FTransform SpawnTransform = UKismetMathLibrary::MakeTransform(FVector(SpawnPoints[Index].X,SpawnPoints[Index].Y,-800.f),FRotator::ZeroRotator);

		UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendCone(DynamicMesh,FGeometryScriptPrimitiveOptions(),SpawnTransform,Radius,Radius/4.f,1300.f,32,1.f);

		//Create markers for use with spawning (Useful Later on)
		if (SpawnMarkers)
		{
			GetWorld()->SpawnActor<AActor>(SpawnMarker,FTransform(SpawnPoints[Index]));
		}
	}

	// Add a box to the base to join the cylinders together
	const FTransform BoxSpawnTransform = FTransform(FVector(0.f,0.f,-800.f));
	UDynamicMesh* BoxDynamicMesh = UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox(DynamicMesh,FGeometryScriptPrimitiveOptions(),BoxSpawnTransform,MaxSpawnDistance + 10000.0, MaxSpawnDistance + 10000.0,400.f);

	// Solidify mesh, smooth/tesselate/calc normals
	FGeometryScript3DGridParameters GridParameters;
	GridParameters.GridCellSize = 0.25f;
	GridParameters.GridResolution = PlatformSwitch(60,50);
	FGeometryScriptSolidifyOptions Options;
	Options.GridParameters = GridParameters;
	Options.ExtendBounds = 0.f;
	Options.SurfaceSearchSteps = 64;
	Options.bThickenShells = false;
	Options.ShellThickness = 1.f;
	
	UDynamicMesh* SolidifyMesh = UGeometryScriptLibrary_MeshVoxelFunctions::ApplyMeshSolidify(BoxDynamicMesh,Options,nullptr);
	UDynamicMesh* PerVertexNormals = UGeometryScriptLibrary_MeshNormalsFunctions::SetPerVertexNormals(SolidifyMesh,nullptr);

	FGeometryScriptIterativeMeshSmoothingOptions GeometryScriptIterativeMeshSmoothingOptions;
	GeometryScriptIterativeMeshSmoothingOptions.NumIterations =6;
	GeometryScriptIterativeMeshSmoothingOptions.Alpha = 0.2f;
	GeometryScriptIterativeMeshSmoothingOptions.EmptyBehavior = EGeometryScriptEmptySelectionBehavior::FullMeshSelection;
	UDynamicMesh* SmoothinoMesh = UGeometryScriptLibrary_MeshDeformFunctions::ApplyIterativeSmoothingToMesh(PerVertexNormals,FGeometryScriptMeshSelection(),GeometryScriptIterativeMeshSmoothingOptions);

	UDynamicMesh* PNTessellationMesh = UGeometryScriptLibrary_MeshSubdivideFunctions::ApplyPNTessellation(SmoothinoMesh, FGeometryScriptPNTessellateOptions(),PlatformSwitch(0,2));
	
	//Delete the section of the mesh not needed any more (Underside)
	FGeometryScriptMeshPlaneCutOptions GeometryScriptMeshPlaneCutOptions;
	GeometryScriptMeshPlaneCutOptions.bFillHoles = false;
	GeometryScriptMeshPlaneCutOptions.bFillSpans = false;
	GeometryScriptMeshPlaneCutOptions.bFlipCutSide = false;
	GeometryScriptMeshPlaneCutOptions.UVWorldDimension = 1.f;
	UDynamicMesh* MeshPlaneCut = UGeometryScriptLibrary_MeshBooleanFunctions::ApplyMeshPlaneCut(PNTessellationMesh,FTransform(FRotator(180.f,0.f,0.f),FVector(0.f,0.f,-390.f)),GeometryScriptMeshPlaneCutOptions);

	//Flatten the top and Project uvs
	UDynamicMesh* UVSMeshPlaneCut = UGeometryScriptLibrary_MeshBooleanFunctions::ApplyMeshPlaneCut(MeshPlaneCut,FTransform(), FGeometryScriptMeshPlaneCutOptions());
	
	UGeometryScriptLibrary_MeshUVFunctions::SetMeshUVsFromPlanarProjection(UVSMeshPlaneCut,0,FTransform(FRotator(),FVector(),FVector(100.f,100.f,100.f)), FGeometryScriptMeshSelection());

	//Release all computer meshes and move the island slightly to retrigger nav mesh gen
	ReleaseAllComputeMeshes();
	AddActorWorldOffset(FVector(0.f,0.f,0.05f));

	if (IIslandInterface* Interface = Cast<IIslandInterface>(UGameplayStatics::GetGameMode(this)))
	{
		Interface->IslandGenComplete();
	}
	
	
	// Set Island Col
	check(MaterialParameterCollection);
	FLinearColor LinearColor = UKismetMaterialLibrary::GetVectorParameterValue(this,MaterialParameterCollection,FName("GrassColour"));
	float H;
	float S;
	float V;
	float A;
	UKismetMathLibrary::RGBToHSV(LinearColor,H,S,V,A);
	FLinearColor ParamaterValue = UKismetMathLibrary::HSVToRGB(102.999725 + UKismetMathLibrary::RandomFloatInRangeFromStream(Seed,0.f,90.f),S,V,A );
	UKismetMaterialLibrary::SetVectorParameterValue(this,MaterialParameterCollection,FName("GrassColour"),ParamaterValue);
	
}
