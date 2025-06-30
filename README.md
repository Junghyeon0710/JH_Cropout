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


## 목차
### [1. Procedural Island Generator](#procedural-island-generator) <br> 
### [2. Spawner 시스템](#spawner-시스템) <br> 
### [3. Common UI](#Common-UI) <br> 
### [4. Behavior Trees](#Behavior-Trees) <br>  


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
UActorComponent* NewActorComp = TemplateData ? CreateComponentFromTemplateData(TemplateData) : CreateComponentFromTemplate(Template);Add commentMore actions

	if (!bDeferredFinish)
	{Add commentMore actions
		FinishAddComponent(NewActorComp, bManualAttachment, RelativeTransform);
	}

	return NewActorComp;
}
```
- 해당 정의쪽 보면 UBlueprintGeneratedClass*로 되어있으며 FindComponentTemplateByName로 Templ듬

<br>

### 저장하기 (비동기)
```C++
void UJHGameInstance::SaveGame()
{
	FAsyncSaveGameToSlotDelegate Delegate;
	Delegate.BindWeakLambda(this, [this](const FString&, int32, bool Success) {
		bHasSave = Success;
	});

	UGameplayStatics::AsyncSaveGameToSlot(SaveGameRef, SaveName, 0, Delegate);
}
```
- 비동기로 게임을 저장, 저장이 완료되면 람다로 함수 호출

<br>

### 데이터 업데이트 예시
Interactable 오브젝트 저장
```C++
void UJHGameInstance::UpdateAllInteractables()
{
	SaveGameRef->Interactables.Empty();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, InteractableActorClass, Actors);

	for (AActor* Actor : Actors)
	{
		AInteractable* Interactable = Cast<AInteractable>(Actor);
		FSaveInteract Interact;
		Interact.Location = Interactable->GetTransform();
		Interact.Health = Interactable->GetProgressionState();
		Interact.Type = Interactable->GetClass();
		Interact.Tag = Interactable->Tags.IsValidIndex(0) ? Interactable->Tags[0] : FName();
		
		SaveGameRef->Interactables.Add(Interact);
	}
}
```
- 월드에 존재하는 모든 상호작용 가능한 액터 정보를 탐색
- 해당 데이터를 `UJHSaveGame`의 `Interactables` 배열에 저장

<br>
  
# Common UI
`Cropout` 프로젝트는 언리얼의 Common UI 시스템을 활용하여  
씬 전환, 로딩, 효과적인 HUD 관리를 처리합니다.

<br>

## ULayer_Menu_ActivatableWidget
>코드

```C++
class UCommonActivatableWidgetStack;
UCLASS()
class JH_CROPOUT_API ULayer_Menu_ActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
public:
	void OnActivate();
protected:

	virtual void NativeOnActivated() override;
	
	void AddStackItem(const TSubclassOf<UCommonActivatableWidget>& InActivatableWidgetClass);
	
	UPROPERTY(meta = (BindWidget) , BlueprintReadOnly)
	TObjectPtr<UCommonActivatableWidgetStack> MainStack;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMainMenuActivatableWidget> ActivatableWidgetClass;
};
```

- `Cropout` 프로젝트에서 **메인 메뉴 계층 관리**를 담당하는 `Common UI` 기반 위젯 클래스입니다.  
- `UCommonActivatableWidgetStack`을 이용해 다양한 메뉴 위젯을 스택 방식으로 관리합니다.
- `AddStackItem()`으로 메뉴 스택을 생성하고, 메인 메뉴 위젯을 추가하는 역할

<br>

## UMainMenuActivatableWidget
Cropout 프로젝트의 **메인 메뉴 위젯 클래스**입니다.  
Unreal의 **CommonUI 시스템**을 기반으로 구성되어 있으며, 세이브 여부에 따라 게임 진행, 새 게임, 종료, 기부 등의 기능을 제공합니다.
>UMG
![Image](https://github.com/user-attachments/assets/1dff38ac-626f-4735-9fd0-4e1e5756a55f)
- 레이아웃 버튼들은 UPROPERTY(meta=(BindWidget), BlueprintReadWrite )같이 전부 바인드 처리

```C++
void UMainMenuActivatableWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	//Reset focus on active (Get Desired Focus Target Is overriden function)
	if (IsValid(GetDesiredFocusTarget()))
	{
		GetDesiredFocusTarget()->SetFocus();
	};
	

	//Check if save exists
	if (UGameplayStatics::GetGameInstance(this)->Implements<UJHGameInstanceInterface>())
	{
		if (IJHGameInstanceInterface* Interface = Cast<IJHGameInstanceInterface>(UGameplayStatics::GetGameInstance(this)))
		{
			bHasSave = Interface->CheckSaveBool();
			BTN_Continue->SetIsEnabled(bHasSave); 
		}
	}

	//Set Donate to only visible on mobile
	const FString PlatformName = UGameplayStatics::GetPlatformName();
	if (PlatformName == "Android" || PlatformName == "iOS" )
	{
		BTN_Donate->SetVisibility(ESlateVisibility::Visible);
	}
	BTN_Donate->SetVisibility(ESlateVisibility::Collapsed);

	
	// Button Events Bind
	//Continue Game
	BTN_Continue->OnClicked().AddLambda([this]()
	{
		if (!Level.IsNull())
		{
			UJHBlueprintFunctionLibrary::GetJhGameInstance(this)->OpenLevel(Level);
		}
	});

	//If save game already exists, prompt for new game
	BTN_NewGame->OnClicked().AddLambda([this]()
	{
		if (bHasSave)
		{
			if (StackRef.IsValid())
			{
				const FString Question = FString::Printf(TEXT("Starting a new game will override your current save. Do you want to continue?"));
				UPromptActivatableWidget* PromptActivatableWidget = StackRef->AddWidget<UPromptActivatableWidget>(PromptWidgetClass);
				PromptActivatableWidget->PromptQuestion = FText::FromString(Question);
				PromptActivatableWidget->OnCallConfirm.BindUObject(this,&ThisClass::ConfirmNewGame);
			}
		}
		else
		{
			ConfirmNewGame();
		}
	});

	// If save game already exists, prompt for new game
	BTN_Donate->OnClicked().AddLambda([this]()
	{
		if(bHasSave)
		{
			const FString Question = FString::Printf(TEXT("Would you like to donate £1.99 to help make the game better? "));
			UPromptActivatableWidget* PromptActivatableWidget = StackRef->AddWidget<UPromptActivatableWidget>(PromptWidgetClass);
			PromptActivatableWidget->PromptQuestion = FText::FromString(Question);
			PromptActivatableWidget->OnCallConfirm.BindUObject(this, &ThisClass::ConfirmDonate);
		}
		else
		{
			ConfirmNewGame();
		}
	});

	//Add Prompt and set question
	BTN_Quit->OnClicked().AddLambda([this]()
	{
		const FString Question = FString::Printf(TEXT("Are you sure you want to quit?"));
		UPromptActivatableWidget* PromptActivatableWidget = StackRef->AddWidget<UPromptActivatableWidget>(PromptWidgetClass);
		PromptActivatableWidget->PromptQuestion = FText::FromString(Question);
		PromptActivatableWidget->OnCallConfirm.BindUObject(this,&ThisClass::ConfirmQuit);
	});

	// ~Button Events Bind
}
```
- 초기화 함수 실행시 버튼 클릭 이벤트 람다함수, 바인드

<br>

## Common UI InputTable
- `InputTable`은 `FCommonInputActionDataBase` 구조체 기반의 **DataTable**
- UI 버튼에 **InputTag**를 설정하면 키보드/게임패드/모바일에 맞는 아이콘과 키 이름을 자동으로 표시

<br>

## 사용 방법
> 사용 위젯

![Image](https://github.com/user-attachments/assets/38b4f8cd-10b2-488b-ad26-de0f6e2a4a32)

- 사용할 위젯에 테이블을 설정

> Table
![Image](https://github.com/user-attachments/assets/2a40725a-aa1c-4da2-a796-b4157af40ed4)

- 인풋 키 바인딩을 설정
- Keyborad : BackSpace, Gamepad : Gamepad face Button Right
- 해당 키 인풋시 자동 실행
	


# Behavior Trees
Cropout은 AI의 행동을 Unreal Engine의 **Behavior Tree (BT)** 시스템을 활용해 구성

---

| 구성 요소        | 설명 |
|------------------|------|
| **Behavior Tree** | AI의 행동 결정 로직을 트리 형태로 정의 |
| **Blackboard**   | 트리에서 사용하는 변수 저장소 (예: Target, Location 등) |
| **Task Nodes**   | AI가 수행할 실제 작업 정의 (예: 이동, 대기, 수확 등) |
| **Decorator**    | 조건 판단 (예: 체력이 50% 이상이면 진행) |
| **Service**      | 주기적으로 상태 체크/갱신 (예: 플레이어 감지) |

---

> Tasks는 C++ 로 구현되어있습니다.

![image](https://github.com/user-attachments/assets/117fe8c8-7f24-4458-addd-2a07fcce68d0)

<br>


### BT_Idle
![Image](https://github.com/user-attachments/assets/5718d50e-38d7-4652-8160-658bca6c5a50)

- EQS로 타운 센터(마을 중심) 주변에서 무작위 지점을 찾아 그곳으로 이동
- 원래 시퀀스(행동 순서)가 실패하면, AI를 타운 센터 주변의 새로운 위치로 리셋

<br>

### BT_Build

![Image](https://github.com/user-attachments/assets/65e21b91-1fac-4ea2-9d90-dbbab98155f0)

- 초기 데이터를 받아서 작업을 준비
- 목표 건물 위치로 이동한 다음 건설을 진행
- 만약 새로운 건물을 더 이상 찾지 못하면 행동을 멈추고 대기 상태로 전환

<br>

### BT_CollectResource
Cropout에서 마을 주민의 자원 수집 행동

![Image](https://github.com/user-attachments/assets/b826c10a-36d7-47b8-b6c5-7707cfd68257)
- 1. 유효한 자원과 수집 지점이 있는지 확인
   - 있음: 자원 위치로 이동하여 수집
   - 없음: 기본 상태(Idle)로 돌아감
2. 자원 위치로 이동 후 자원을 수집
3. 수집 지점으로 이동하여 자원을 전달
4. 자원 또는 수집 지점이 중간에 유효하지 않게 된 경우:
   - 다음 유효한 자원 및 수집 지점을 찾음
   - 찾지 못하면 기본 상태(Idle)로 복귀

<br>

### BT_Farming
Cropout 게임의 마을 주민이 수행하는 작물 관련 AI 행동 트리

![Image](https://github.com/user-attachments/assets/a797c2de-a493-4def-891b-340b8d057633)

1. 수확할 자원(작물) 또는 수집 지점이 없으면 기본 상태(Idle)로 복귀
   - 마을 주민은 자원 수집을 반복하지만,
     자원이나 수집 지점이 사라지면 새로운 곳을 탐색함
2. 작물 사이클의 각 단계를 순차적으로 처리
   - 파종 → 성장 → 수확
3. 작물 수확 (Harvest Crop)
   - 수확 가능한 작물 위치로 이동
   - 수확 작업 수행
4. 파밍 플롯(Farm Plot) 찾기
   - 이미 작물이 심어진 상태이거나, 수확 가능한 상태
5. 사용할 수 있는 작물 위치 탐색
6. 사용할 수 있는 대상(Target) 탐색


<br>

## Behavior Trees 불러오기
AJH_Villager::ChangeJob

``` C++
void AJH_Villager::ChangeJob(FName NewJob)
{
	Job = NewJob;

	if (!JobDataTable)
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,TEXT("ERROR: Failed to Load Job"));
		return;
	}
	
	//Using Tags makes it much easier to quickly find actors with set properties without having to explicitly cast.
	const FJob* JobData = JobDataTable->FindRow<FJob>(NewJob,TEXT(""));
	if (!JobData)
	{
		return;
	}

	if (Tags.Find(NewJob) != -1)
	{
		return;
	}

	if (Tags.IsEmpty())
	{
		Tags.Add(NewJob);
	}
	else
	{
		Tags[0] = NewJob;
	}

	ResetJobState();

	// The data table only stores soft references, which avoids loading in every possible behavior tree, tool and hat.

	// This means we can store every job type in a single graph and not worry about loading in content that won't be used on the map.

	FSoftObjectPath SoftObjectPath(JobData->BehaviorTree.ToSoftObjectPath());
	UAssetManager::GetStreamableManager().RequestAsyncLoad(SoftObjectPath, FStreamableDelegate::CreateUFunction(this,FName("AsyncLoadBehaviorTree")));

	FSoftObjectPath AnimSoftObjectPath(JobData->WorkAnim.ToSoftObjectPath());
	UAssetManager::GetStreamableManager().RequestAsyncLoad(AnimSoftObjectPath, FStreamableDelegate::CreateUFunction(this,FName("AsyncLoadAnimMontage")));

	FSoftObjectPath HatSoftObjectPath(JobData->Hat.ToSoftObjectPath());
	UAssetManager::GetStreamableManager().RequestAsyncLoad(HatSoftObjectPath, FStreamableDelegate::CreateUFunction(this,FName("AsyncLoadSkeletalMesh")));

	FSoftObjectPath ToolSoftObjectPath(JobData->Tool.ToSoftObjectPath());
	UAssetManager::GetStreamableManager().RequestAsyncLoad(ToolSoftObjectPath, FStreamableDelegate::CreateUFunction(this,FName("AsyncLoadStaticMesh")));
	
}
```
마을 주민의 직업을 설정하고 관련 리소스를 비동기로 로드하는 함수입니다.

- 직업 태그 변경
- 직업 데이터 테이블에서 직업 정보 조회
- 해당 직업에 필요한 에셋 (비헤이비어 트리, 애니메이션, 장비 등) 비동기 로드


