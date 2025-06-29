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
[![Video Thumbnail](https://img.youtube.com/vi/9zKp4cXuJ1Q/0.jpg)](https://www.youtube.com/watch?v=9zKp4cXuJ1Q)

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

# Spawner 시스템

언리얼 엔진용 랜덤 오브젝트/인스턴스 배치 시스템 
네비게이션 데이터 기반으로 클래스 또는 인스턴스를 자동 배치하며, 비동기 로딩과 커스텀 설정을 지원

---

### 비동기 클래스 로드 (Async Load)
`ASpawner::AsyncLoadClass()` 함수는 `SpawnTypes` 배열에 저장된 클래스들을  
비동기(Async) 방식으로 순차적으로 로드하는 기능을 담당

```C++
void ASpawner::AsyncLoadClass()
{
    FSoftObjectPath SoftObjectPath(SpawnTypes[ClassRefIndex].ClassRef.ToSoftObjectPath());

    UAssetManager::GetStreamableManager().RequestAsyncLoad(SoftObjectPath, FStreamableDelegate::CreateLambda([this]()
    {
        ClassRefIndex++;

        if (ClassRefIndex > SpawnTypes.Num() - 1)
        {
            bAsyncComplete = true; // 모든 클래스 로드 완료
        }
        else
        {
            AsyncLoadClass(); // 다음 클래스 로드 재귀 호출
        }
    }));
}
```

- `SpawnTypes[ClassRefIndex].ClassRef`에서 `FSoftObjectPath`를 생성
- `UAssetManager`의 `RequestAsyncLoad`를 호출해 해당 에셋을 비동기 로딩
- 로딩 완료 시 호출되는 람다(Callback) 내에서 다음 클래스 로딩 재귀 호출
- 모든 클래스가 로딩 완료되면 `bAsyncComplete` 플래그를 `true`로 설정

<br>

## 액터 클래스 기반 스폰 함수 (`SpawnAssets`)

`SpawnAssets` 함수는 지정된 액터 클래스를 네비게이션 시스템을 활용해  
맵 내 여러 랜덤 위치에 스폰하는 기능을 담당

```C++
void ASpawner::SpawnAssets(TSubclassOf<AActor> Class, const FSpawnData& SpawnParams)
{
	// Spawn of Class
	int LocalCount = 0;
	int32 BiomeCount = SpawnParams.BiomeCount;
	for (int Index = 0; Index <= BiomeCount ;++Index)
	{
		
		if (UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
		{
			FNavLocation RandomLocation;
			//NavSystem->K2_GetRandomLocationInNavigableRadius()
			NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector,10000.f,RandomLocation,NavData);

			//Pick Points around Biome Points
			int32 RandomCount = UKismetMathLibrary::RandomIntegerFromStream(Seed,SpawnParams.SpawnPerBiome);

			for (int i =0; i <= RandomCount; ++i)
			{
				FNavLocation SpawnPos;
				NavSystem->GetRandomPointInNavigableRadius(RandomLocation.Location,SpawnParams.BiomeScale,SpawnPos);

				FTransform SpawnTransform;
				SpawnTransform.SetLocation(SteppedPosition(SpawnPos.Location));
				SpawnTransform.SetRotation(FRotator(0.f,UKismetMathLibrary::RandomFloatInRange(0,SpawnParams.RandomRotationRange),0.f).Quaternion());
				SpawnTransform.SetScale3D(FVector(UKismetMathLibrary::RandomFloatInRange(1.f,SpawnParams.ScaleRange+1.f)));

				FActorSpawnParameters SpawnParameters;
				SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				AActor* Actor = GetWorld()->SpawnActor<AActor>(Class,SpawnTransform,SpawnParameters);
				LocalCount++;

				if (IIslandInterface* Interface = Cast<IIslandInterface>(Actor))
				{
					Interface->ScaleUp(LocalCount / TotalCounter);
				}
			}
		}
	}
}
```

- `SpawnParams.BiomeCount` 만큼 루프를 돌며 각 바이옴 영역을 대표하는 랜덤 위치 선택  
- 내비게이션 시스템(`UNavigationSystemV1`)을 사용해 `GetRandomPointInNavigableRadius`로 네비게이션 가능한 위치 얻기  
- 각 바이옴 위치 주변 반경(`BiomeScale`) 내에서 `SpawnPerBiome` 만큼 랜덤 위치를 다시 선택하여 액터 스폰  
- 스폰 시 위치는 `SteppedPosition` 함수로 그리드에 맞춰 보정  
- 회전과 스케일은 `SpawnParams`에 정의된 랜덤 범위 내에서 설정  
- 액터가 `IIslandInterface`를 구현하고 있으면, 비율에 따라 `ScaleUp` 콜백 호출

<br>

## Spawner 시스템에서 클론코딩 하다 생긴 문제
> 샘플에서 Spawn Instances를 하는 블프 코드
![Image](https://github.com/user-attachments/assets/f57316ea-cf3f-406d-a8ff-99842d7925de)

Add Instanced Static Mesh Component쪽을 들어가보면
```C++
UFUNCTION(BlueprintCallable, meta=(ScriptNoExport, BlueprintInternalUseOnly = "true", DefaultToSelf="ComponentTemplateContext", InternalUseParam="ComponentTemplateContext,bDeferredFinish"))
ENGINE_API UActorComponent* AddComponent(FName TemplateName, bool bManualAttachment, const FTransform& RelativeTransform, const UObject* ComponentTemplateContext, bool bDeferredFinish = false);
```

블루프린트에서 "Add Instanced Static Mesh Component" 노드를 클릭하면
다음의 C++ 함수 선언부로 이동하게 된다.

- 해당 노드는 런타임에 컴포넌트를 생성하는 함수인 AddComponent()와 연결되어 있으며,
내부적으로 TemplateName을 통해 어떤 컴포넌트를 추가할지 지정한다.

- AddComponent()는 BlueprintInternalUseOnly = "true"와 ScriptNoExport 메타 태그가 설정되어 있어
블루프린트 상에서는 사용자에게 직접 노출되지 않지만,
엔진 내부에서는 자동으로 생성된 노드와 연결된다.

- "Add Instanced Static Mesh Component"라는 노드는
내부적으로 "NODE_AddInstancedStaticMeshComponent"와 같은 이름을 가진
템플릿 기반 컴포넌트를 생성하는 호출이며,
해당 노드가 실행될 때 결국 AddComponent()를 통해 컴포넌트가 실제로 생성

즉 이 함수는 C++로 호출로 해봤자 생성이 안된다.

<br>

## 해결방법
> BP_Spawner

![Image](https://github.com/user-attachments/assets/37a48266-35fc-45d4-a4ac-4f18a7b399fc)

- 블루프린트 BeginPlay에서 Add Instanced Static Mesh Component 노드를 호출해
런타임에 메시 컴포넌트를 동적으로 추가

>실제 사용할 코드
```C++
if (UInstancedStaticMeshComponent* StaticMeshComponent = Cast<UInstancedStaticMeshComponent>(AddComponent(FName("NODE_AddInstancedStaticMeshComponent-3"),false,FTransform(),this)))
 {
	IndexCounter++;
	checkf(SpawnInstances[IndexCounter].ClassRef,TEXT("No StaticMesh"))
	FSpawnInstance Instance = SpawnInstances[IndexCounter];
	StaticMeshComponent->SetStaticMesh(Instance.ClassRef);
	SpawnInst(StaticMeshComponent,Instance.BiomeScale,Instance.BiomeCount,Instance.SpawnPerBiome);
	if (IndexCounter >= SpawnTypes.Num())
    	{
		if (bCallSave)
		{
    	  	  FinishSpawning();
		}
    	}
	else
	{
	   GetWorld()->GetTimerManager().UnPauseTimer(NavCheckHandle);
	}		
}
```
- 그런 다음에 "NODE_AddInstancedStaticMeshComponent"라는 템플릿 이름으로 AddComponent를 호출해줌

>AddComponent.cpp
```C++
UActorComponent* AActor::AddComponent(FName TemplateName, bool bManualAttachment, const FTransform& RelativeTransform, const UObject* ComponentTemplateContext, bool bDeferredFinish)
{
	if (const UWorld* World = GetWorld())
	{
		if (World->bIsTearingDown)
		{
			UE_LOG(LogActor, Warning, TEXT("AddComponent failed for actor: [%s] with param TemplateName: [%s] because we are in the process of tearing down the world")
				, *GetName()
				, *TemplateName.ToString());
			return nullptr;
		}
	}
	else
	{
		UE_LOG(LogActor, Warning, TEXT("AddComponent failed for actor: [%s] with param TemplateName: [%s] because world == nullptr")
			, *GetName()
			, *TemplateName.ToString());
		return nullptr;
	}

	UActorComponent* Template = nullptr;
	FBlueprintCookedComponentInstancingData* TemplateData = nullptr;
	for (UClass* TemplateOwnerClass = (ComponentTemplateContext != nullptr) ? ComponentTemplateContext->GetClass() : GetClass()
		; TemplateOwnerClass && !Template && !TemplateData
		; TemplateOwnerClass = TemplateOwnerClass->GetSuperClass())
	{
		if (UBlueprintGeneratedClass* BPGC = Cast<UBlueprintGeneratedClass>(TemplateOwnerClass))
		{
			// Use cooked instancing data if available (fast path).
			if (BPGC->UseFastPathComponentInstancing())
			{
				TemplateData = BPGC->CookedComponentInstancingData.Find(TemplateName);
			}
			
			if (!TemplateData || !TemplateData->bHasValidCookedData
				|| !ensureMsgf(TemplateData->ComponentTemplateClass != nullptr, TEXT("AddComponent fast path (%s.%s): Cooked data is valid, but runtime support data is not initialized. Using the slow path instead."), *BPGC->GetName(), *TemplateName.ToString()))
			{
				Template = BPGC->FindComponentTemplateByName(TemplateName);
			}
		}
	}

	UActorComponent* NewActorComp = TemplateData ? CreateComponentFromTemplateData(TemplateData) : C행

# Behavior Trees





