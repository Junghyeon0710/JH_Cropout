// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner/Spawner.h"

#include "NavigationSystem.h"
#include "Engine/AssetManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/IslandInterface.h"
#include "Save/JHGameInstanceInterface.h"
#include "GameFramework/GameModeBase.h"


ASpawner::ASpawner()
{

	PrimaryActorTick.bCanEverTick = false;

}

void ASpawner::BeginPlay()
{
	Super::BeginPlay();

	AsyncLoadClasses();

	if (!bAutoSpawn) return;

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this; // 콜백이 발생할 객체
	LatentInfo.UUID = 1;             // 고유 ID
	LatentInfo.Linkage = 0;          // 내부 사용
	LatentInfo.ExecutionFunction = FName("WaitForNavMeshAndAssets");
	UKismetSystemLibrary::DelayUntilNextTick(this, LatentInfo);
 }

void ASpawner::AsyncLoadClasses()
{
	ClassRefIndex = 0;
	bAsyncComplete = false;
	
	AsyncLoadClass();

}

void ASpawner::AsyncLoadClass()
{
	FSoftObjectPath SoftObjectPath(SpawnTypes[ClassRefIndex].ClassRef.ToSoftObjectPath());
	FStreamableDelegate StreamableDelegate;
	UAssetManager::GetStreamableManager().RequestAsyncLoad(SoftObjectPath, FStreamableDelegate::CreateLambda([this, SoftObjectPath]()
	{
		ClassRefIndex++;
		if (ClassRefIndex > SpawnTypes.Num()-1)
		{
			bAsyncComplete = true;
		}
		else
		{
			AsyncLoadClass();
		}
   }));
}

void ASpawner::WaitForNavMeshAndAssets()
{
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,TEXT("sdadsa"));
	Counter = 0;
	IndexCounter = 0;

	GetWorld()->GetTimerManager().SetTimer(
		NavCheckHandle,
		this,
		&ThisClass::ReadyToSpawn,
		.5,
		true,
		-.5f);
}

void ASpawner::ReadyToSpawn()
{
	
	bool bNavigationBeingBuilt = UNavigationSystemV1::GetNavigationSystem(this)->IsNavigationBeingBuilt(this);
	if (bNavigationBeingBuilt && !bAsyncComplete) return;

	GetWorld()->GetTimerManager().PauseTimer(NavCheckHandle);

	// Once finished spawning BP's, spawn the instanced static mesh array
	if (bActorSwitch)
	{
		SpawnAssets(SpawnTypes[IndexCounter].ClassRef.Get(),SpawnTypes[IndexCounter]);
		// Switch Class After Pause
		if (++IndexCounter >=SpawnTypes.Num())
		{
			IndexCounter = 0;
			bActorSwitch = false;
		}
		GetWorld()->GetTimerManager().UnPauseTimer(NavCheckHandle);
	}
	else // Spawn Instances
	{
		if (UInstancedStaticMeshComponent* StaticMeshComponent = Cast<UInstancedStaticMeshComponent>(AddComponent(FName("InstancedStaticMeshComponent"),false,FTransform(),this)))
		{
			checkf(SpawnInstances[IndexCounter].ClassRef,TEXT("No StaticMesh"))
			FSpawnInstance Instance = SpawnInstances[IndexCounter];
			StaticMeshComponent->SetStaticMesh(Instance.ClassRef);
			SpawnInst(StaticMeshComponent,Instance.BiomeScale,Instance.BiomeCount,Instance.SpawnPerBiome);
			IndexCounter++;
			if (++IndexCounter >=SpawnTypes.Num())
			{
				if (bCallSave)
				{
					FinishSpawning();
				}
			}
			GetWorld()->GetTimerManager().UnPauseTimer(NavCheckHandle);
		}
		
	}
	
}

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
			NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector,10000.f,RandomLocation,NavData);

			//Pick Points around Biome Points
			int32 RandomCount = UKismetMathLibrary::RandomIntegerFromStream(Seed,SpawnParams.SpawnPerBiome);

			for (int i =0; i <= RandomCount; ++i)
			{
				FNavLocation SpawnPos;
				NavSystem->GetRandomPointInNavigableRadius(RandomLocation.Location,SpawnParams.BiomeScale,SpawnPos);

				FTransform SpawnTransform;
				SpawnTransform.SetLocation(SteppedPosition(SpawnPos.Location));
				SpawnTransform.SetRotation(FRotator(0.f,SpawnParams.RandomRotationRange,0.f).Quaternion());
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

FVector ASpawner::SteppedPosition(const FVector& InParam) const
{
	const FVector DivideVector =  UKismetMathLibrary::Divide_VectorFloat(InParam,200.f);
	
	const float Value = 200.f; 
	return FVector(UKismetMathLibrary::Round(DivideVector.X) * Value,UKismetMathLibrary::Round(DivideVector.Y) * Value, 0.f );
}

void ASpawner::SpawnInst(UInstancedStaticMeshComponent* Class, float Radius, int32 BiomeCount, int32 MaxPawn)
{
	//Loop Of Initial Biome Points
	int32 LocalCount = 0;
	for (int i =0;i<= BiomeCount; ++i)
	{
		if (UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
		{
			FNavLocation Pos;
			NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector,10000.f,Pos,NavData);
			//Pick Points around Biome Points
			int32 RandomIndex = UKismetMathLibrary::RandomIntegerFromStream(Seed,MaxSpawn);
			for (int i2 = 0; i2<= RandomIndex; ++i2 )
			{
				FNavLocation SpawnPos;
				NavSystem->GetRandomPointInNavigableRadius(Pos,Radius,SpawnPos,NavData);
				FTransform InstanceTransform;
				InstanceTransform.SetLocation(FVector(SpawnPos.Location.X,SpawnPos.Location.Y,0.f));
				InstanceTransform.SetScale3D(FVector(UKismetMathLibrary::Lerp(0.8,1.5,(Pos.Location - SpawnPos.Location).Size() / Radius)));
				
				Class->AddInstance(InstanceTransform);
				LocalCount++;
			}
		}
	}
}

void ASpawner::FinishSpawning() const
{
	if (IJHGameInstanceInterface* Interface = Cast<IJHGameInstanceInterface>(UGameplayStatics::GetGameInstance(this)))
	{
		Interface->UpdateAllInteractables();
	}
	
	if (IIslandInterface* Interface = Cast<IIslandInterface>(UGameplayStatics::GetGameMode(this)))
	{
		Interface->SpawningComplete();
	}
}

