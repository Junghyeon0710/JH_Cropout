// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/JHGameMode.h"

#include "NavigationSystem.h"
#include "Blueprint/AIAsyncTaskBlueprintProxy.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Engine/AssetManager.h"
#include "GameMode/JHBlueprintFunctionLibrary.h"
#include "GameMode/JHGameInstance.h"
#include "Kismet/BlueprintMapLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/SpawnMarker.h"
#include "Spawner/Spawner.h"
#include "UI/Game/Layer_Game_ActivatableWidget.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"


AJHGameMode::AJHGameMode()
{
	
}

void AJHGameMode::BeginPlay()
{
	Super::BeginPlay();

	//Set loading screen to animate out, reset render target (로딩 화면을 애니메이션으로 설정하고 렌더링 대상을 재설정합니다)
	Cast<UJHGameInstance>(UJHBlueprintFunctionLibrary::GetJhGameInstance(this))->TransitionOut();

	StartTime = UKismetSystemLibrary::GetGameTimeInSeconds(this);

	check(RenderTarget2D);
	UKismetRenderingLibrary::ClearRenderTarget2D(this,RenderTarget2D,FLinearColor::Black);

	//Setup Island
	GetSpawnRef();

	// Add Game HUD to screen
	checkf(Layer_Game_ActivatableWidgetClass,TEXT("No Layer_Game_ActivatableWidgetClass"))
	UIHUD = CreateWidget<ULayer_Game_ActivatableWidget>(GetWorld(),Layer_Game_ActivatableWidgetClass);
	UIHUD->AddToViewport();
	UIHUD->ActivateWidget();

}

void AJHGameMode::GetSpawnRef()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this,ASpawner::StaticClass(),Actors);
	SpawnerRef = Cast<ASpawner>(Actors[0]);
}

void AJHGameMode::AddResource(EResourceType Resource, int32 Value)
{
	const int32* FoundValue = Resources.Find(Resource);
	const int32 FValue = FoundValue ? *FoundValue : 0;

	Resources.Add(Resource, FValue + Value);
	
	OnUpdateResources.Broadcast(Resource,FValue + Value);

	if(IJHGameInstanceInterface* JIF = Cast<IJHGameInstanceInterface>(UGameplayStatics::GetGameInstance(this)))
	{
		JIF->UpdateAllResources(Resources);
	}
	
}

bool AJHGameMode::CheckResource(EResourceType Resource, int32& OutValue)
{
	if (Resources.Contains(Resource))
	{
		OutValue = *Resources.Find(Resource);  // 이제 안전하게 역참조 가능
		return true;
	}
	OutValue = 0;
	return Resources.Contains(Resource);
	
}

void AJHGameMode::RemoveTargetResource(EResourceType Resource, int32 InValue)
{
	//Update the target resource
	const int32* FindValue = Resources.Find(Resource);
	Resources.Add(Resource,*FindValue - InValue);
	if(OnUpdateResources.IsBound())
	{
		OnUpdateResources.Broadcast(Resource,*FindValue);
	}

	//If food drops below 0, show end game screen with Lose condition
	const int32* FoodValue = Resources.Find(EResourceType::Food);
	if(*FoodValue <=0)
	{
		EndGame(false);
	}
}

TMap<EResourceType, int32> AJHGameMode::GetCurrentResources()
{
	return Resources;
}

void AJHGameMode::AddUI(TSubclassOf<UCommonActivatableWidget> Widget)
{
	UIHUD->AddStackItem(Widget);
}

void AJHGameMode::RemoveCurrentUILayer()
{
	UIHUD->PullCurrentActiveWidget();
}

void AJHGameMode::IslandGenComplete()
{
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this; // 콜백이 발생할 객체
	LatentInfo.UUID = 1;             // 고유 ID
	LatentInfo.Linkage = 0;          // 내부 사용
	LatentInfo.ExecutionFunction = FName("LoadOrSpawnIslandAssets");
	
	UKismetSystemLibrary::DelayUntilNextTick(this, LatentInfo);
}

void AJHGameMode::LoadOrSpawnIslandAssets()
{
	//Once Island has finished building, check if save file exists. If so, load data from Save file. If not, being spawning assets
	UGameInstance* GI =  UGameplayStatics::GetGameInstance(this);
	if (IJHGameInstanceInterface* Interface = Cast<IJHGameInstanceInterface>(GI))
	{
		//TODO 
		if (Interface->CheckSaveBool())
		{
			
		}
		else
		{
			BeginAsyncSpawning();
		}
	}
}

void AJHGameMode::BeginAsyncSpawning()
{
	//Spawn Town Hall
	FSoftObjectPath SoftObjectPath(TownHall_Ref.ToSoftObjectPath());
	FStreamableDelegate Delegate;
	UAssetManager::GetStreamableManager().RequestAsyncLoad(SoftObjectPath, FStreamableDelegate::CreateLambda([this, SoftObjectPath]()
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(this,ASpawnMarker::StaticClass(),Actors);
		AActor* RandomActor = Actors[FMath::RandRange(0,Actors.Num()-1)];
		if(RandomActor)
		{
			FTransform SpawnTransform = FTransform(UJHBlueprintFunctionLibrary::SteppedPosition(RandomActor->GetActorLocation()));
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Undefined;
			TownHall = GetWorld()->SpawnActor<AActor>(TownHall_Ref.Get(),SpawnTransform,SpawnParameters);

			//Create 3 villagers
			for(int i =0; i<=2 ; i++)
			{
				SpawnVillager();
			}

			//Update Villagers
			if(OnUpdateVillagers.IsBound())
			{
				OnUpdateVillagers.Execute(VillagerCount);
			}

			if(IJHGameInstanceInterface* JIF = Cast<IJHGameInstanceInterface>(UGameplayStatics::GetGameInstance(this)))
			{
				JIF->UpdateAllVillagers();
			}

			//Spawn Interactables
			SpawnerRef->SpawnRandom();
		}
	}));
}

void AJHGameMode::SpawnVillager()
{
	if (!TownHall) return; // TownHall이 nullptr이면 실행하지 않음.

	// 마을회관의 월드 위치(Origin) 및 크기(Extent) 가져오기
	FVector Origin, Extent;
	TownHall->GetActorBounds(false, Origin, Extent);

	// 마을회관 반경 내에서 랜덤 위치 생성
	const float MinExtent = FMath::Min(Extent.X, Extent.Y);
	FVector RandomOrigin = Origin + (UKismetMathLibrary::RandomUnitVector() * MinExtent * 2);
	RandomOrigin.Z = 0; // 높이값 초기화 (지면 위에서 스폰하도록)

	// 랜덤 위치 근처에서 이동 가능한 네비게이션 메시(NavMesh) 위치 찾기
	FNavLocation ResultLocation;
	const UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    
	if (!NavSystem || !NavSystem->GetRandomReachablePointInRadius(RandomOrigin, 500.f, ResultLocation))
	{
		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red, TEXT("유효한 스폰 위치를 찾을 수 없음"));
		return;
	}

	// VillagerClass가 설정되어 있는지 확인 후 AI 스폰
	checkf(VillagerClass, TEXT("No VillagerClass"));
	UAIBlueprintHelperLibrary::SpawnAIFromClass(this, VillagerClass, nullptr, ResultLocation);

	VillagerCount++;
}

void AJHGameMode::EndGame(bool bIsWin)
{
	//UI Interactions
	static bool bIsOne = false;

	if(!bIsOne)
	{
		FTimerHandle DelayTimer;
		GetWorld()->GetTimerManager().SetTimer(DelayTimer,FTimerDelegate::CreateWeakLambda(this,[this,bIsWin]()
		{
			UIHUD->EndGame(bIsWin);
		}),3,false);
	}
}


