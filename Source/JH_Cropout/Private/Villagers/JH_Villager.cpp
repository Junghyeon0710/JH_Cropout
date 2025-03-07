// Fill out your copyright notice in the Description page of Project Settings.


#include "Villagers/JH_Villager.h"

#include <future>

#include "AIController.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/DataTable.h"
#include "Engine/Engine.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameMode/ResourceInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameMode.h"
#include "Engine/SkeletalMesh.h"
#include "Save/JHGameInstanceInterface.h"
#include "Villagers/Job.h"

AJH_Villager::AJH_Villager()
{
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(Capsule);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);

	Tool = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tool"));
	Tool->SetupAttachment(SkeletalMesh,FName("hand_rSocket"));

	Hat = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hat"));
	Hat->SetupAttachment(SkeletalMesh,FName("headSocket"));

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

	//Set Current Tag as Job
	ChangeJob(TEXT("Idle"));
}

void AJH_Villager::Eat() const
{
	if(IResourceInterface* Interface = Cast<IResourceInterface>( GetWorld()->GetAuthGameMode()))
	{
		Interface->RemoveTargetResource(EResourceType::Food,3);
	}
	
}

void AJH_Villager::Action(AActor* Actor)
{
	if (!IsValid(Actor))
	{
		return;
	}
	TargetRef = Actor;
	if (Actor->Tags.IsValidIndex(0))
	{
		ChangeJob(Actor->Tags[0]);
		if(IJHGameInstanceInterface* Interface = Cast<IJHGameInstanceInterface>(GetGameInstance()))
		{
			Interface->UpdateAllVillagers();
		}
	}
	
}

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

void AJH_Villager::ReturnToDefaultBT()
{
	ChangeJob(FName("Idle"));
}

void AJH_Villager::PlayWorkAnim(float Delay)
{
	PlayVillagerAnim(WorkAnim,Delay);
	Tool->SetStaticMesh(TargetTool);
	Tool->SetVisibility(true);
	
}

float AJH_Villager::PlayDeliverAnim()
{
	check(VillagerMontage);
	PlayVillagerAnim(VillagerMontage,1);
	return 1;
}

void AJH_Villager::AddResource(EResourceType Resource, int32 Value)
{
	//Set currently held resource and value
	ResourcesHeld = Resource;
	Quantity = Value;

	//Set Box to visible
	Tool->SetVisibility(true);
	check(CrateMesh);
	Tool->SetStaticMesh(CrateMesh);
}

void AJH_Villager::RemoveResource(EResourceType& OutResource, int32& OutValue)
{
	//Store Resource type and value locally
	EResourceType CacheResource = ResourcesHeld;
	int32 CacheValue = Quantity;

	//Clear resource and value
	ResourcesHeld = EResourceType::None;
	Quantity = 0;

	//Hide Crate
	Tool->SetVisibility(false);
	Tool->SetStaticMesh(nullptr);

	//Send locally stored values
	OutResource = CacheResource;
	OutValue = CacheValue;
}

void AJH_Villager::AsyncLoadBehaviorTree()
{
	AAIController* AC = Cast<AAIController>(GetController());
	if (!AC)
	{
		return; 
	}

	
	if (UBehaviorTree* BehaviorTree = JobDataTable->FindRow<FJob>(Job,TEXT(""))->BehaviorTree.Get())
	{
		AC->RunBehaviorTree(BehaviorTree);
		ActiveBehavior = BehaviorTree;
	}

	if (IsValid(TargetRef))
	{
		AC->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"),TargetRef);
	}
}

void AJH_Villager::AsyncLoadAnimMontage()
{
	if (UAnimMontage* Montage = JobDataTable->FindRow<FJob>(Job,TEXT(""))->WorkAnim.Get())
	{
		WorkAnim = Montage;
	}
}

void AJH_Villager::AsyncLoadSkeletalMesh()
{
	if (USkeletalMesh* Skeletal= JobDataTable->FindRow<FJob>(Job,TEXT(""))->Hat.Get())
	{
		Hat->SetSkeletalMeshAsset(Skeletal);
		Hat->SetVisibility(true);
	}
}

void AJH_Villager::AsyncLoadStaticMesh()
{
	if (UStaticMesh* StaticMesh = JobDataTable->FindRow<FJob>(Job,TEXT(""))->Tool.Get())
	{
		TargetTool = StaticMesh;
	}
}

void AJH_Villager::ResetJobState()
{
	StopJob();

	// Remove Hat
	Hat->SetSkeletalMeshAsset(nullptr);
	Hat->SetVisibility(false);
	
	//Remove Tool
	Tool->SetVisibility(false);
	Tool->SetStaticMesh(nullptr);
	TargetTool = nullptr;
	
}

void AJH_Villager::StopJob()
{
	Tool->SetVisibility(false);
	SkeletalMesh->GetAnimInstance()->Montage_StopGroupByName(0,TEXT("DefaultGroup"));

	if (AAIController* AIController  = Cast<AAIController>(GetController()) ; IsValid(AIController))
	{
		AIController->StopMovement();
	}

	Quantity = 0;
	
}

void AJH_Villager::PlayVillagerAnim(UAnimMontage* Montage, float Length)
{
	UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		// 애니메이션 재생
		AnimInstance->Montage_Play(Montage);

		// 애니메이션 종료 이벤트 바인딩
		FOnMontageEnded EndDelegate;
		EndDelegate.BindUObject(this, &AJH_Villager::OnMontageEnded);
		AnimInstance->Montage_SetEndDelegate(EndDelegate, Montage);
	}

	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer,FTimerDelegate::CreateLambda([AnimInstance]()
	{
		AnimInstance->Montage_StopGroupByName(0,FName("DefaultGroup"));
	}),Length,false);
	
}

void AJH_Villager::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	Tool->SetVisibility(false);
}


