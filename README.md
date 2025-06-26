# Unreal5 : JH_Cropout
학습 목적으로 제작된 Unreal Engine 5의 대표적인 게임 [Cropout Sample](https://www.unrealengine.com/en-US/blog/cropout-casual-rts-game-sample-project) 샘플 클론 프로젝트입니다.
블루프린트로 제작 된 샘플을 C++로 만들었습니다.

<br>

- 엔진 : Unreal Engine 5.4
- IDE	: Rider
- 툴	: C++
- 인원 : 개인 개발
- UI	 : Common UI
- 언리얼 엔진 프레임워크 : Behavior Trees , EQS , Saving and Loading ,  Blueprint Interface, Geometry Script

<br>

## Cropout에서 배울 수 있는 것들

Cropout은 언리얼 엔진 5 기반의 캐주얼 톱다운 RTS 게임 샘플 프로젝트입니다. 다음과 같은 핵심 개념을 학습할 수 있습니다:

### 1. 크로스 플랫폼 최적화
- 모바일, 콘솔, PC를 아우르는 멀티 플랫폼 대응 구조
- 성능 차이에 따른 콘텐츠 품질 조절 및 스케일링

### 2. 블루프린트 기반 구조
- 대부분의 게임 로직이 블루프린트로 구현
- 비주얼 스크립팅 학습에 최적화된 예제

### 3. UE5 최신 기능 활용
- Common UI: 다양한 입력 디바이스를 지원하는 UI 프레임워크
- Enhanced Input: 입력 매핑 컨텍스트를 통해 유연한 입력 제어
- Geometry Script: 프로시저 방식의 섬(지형) 생성 예시

### 4. 게임플레이 시스템 구성
- Behavior Tree, EQS를 이용한 AI 시스템
- 저장 및 불러오기 시스템, Blueprint Interface 활용

### 5. 플랫폼별 패키징 최적화
- 모바일/콘솔/PC 환경별 리소스 품질 조정
- 텍스처, 메시, 파티클 등의 디테일 차등 적용

### 6. 플러그인 구조 활용
- 모듈 단위 설계로 유지보수와 재사용성이 높은 구조

<br>


## 영상

<br>

# Procedural Island Generator 
언리얼 엔진의 **Geometry Script 기능**을 활용하여 실시간으로 **섬 형태의 메시를 생성**하는 예제입니다.  
DynamicMesh 시스템을 활용해 **지형을 만들고, 솔리디파이(Solidify), 노멀 보정, 평탄화 및 UV 프로젝션**까지 처리합니다.

```C++
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
```
-  랜덤 지형 생성을 위한 Seed 기반 `FRandomStream`
-  여러 개의 실린더를 기반으로 섬 지형 구성
-  Solidify, Smooth, Tessellate, Plane Cut 등 Geometry Script 단계별 처리
-  MaterialParameterCollection을 통해 지형 색상 HSV 기반 랜덤화
-  디버깅용 Spawn Marker 생성 가능
-  GameMode의 인터페이스 콜백 (`IIslandInterface`) 연동

TODO : Gif

<br>

# Procedural Island Generator & Spawner System

언리얼 엔진 5의 Geometry Script와 네비게이션 시스템을 활용한 **프로시저럴 섬 생성 및 스폰 시스템**입니다.  
게임 시작 시 섬을 생성하고, 네비게이션 빌드가 완료되면 자동으로 액터나 인스턴스를 배치합니다.

---

## 특징

### 1. 프로시저럴 섬 생성

- Geometry Script를 사용한 실시간 메시 생성
- 실린더 + 박스 결합 후 솔리디파이(Solidify), 테셀레이션, UV 생성
- 지형 부드럽게(Smoothing), 아래 잘라내기(Cut), 컬러 랜덤화 처리
- `DynamicMeshComponent` 기반의 완전 절차적 메시




