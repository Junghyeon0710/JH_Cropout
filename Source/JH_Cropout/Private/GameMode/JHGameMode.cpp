// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/JHGameMode.h"

#include "Blueprint/UserWidget.h"
#include "GameMode/JHBlueprintFunctionLibrary.h"
#include "GameMode/JHGameInstance.h"
#include "Kismet/BlueprintMapLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Spawner/Spawner.h"
#include "UI/Game/Layer_Game_ActivatableWidget.h"

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
	Resources.Add(Resource, *FoundValue + Value);

	OnUpdateResources.Execute(Resource,*FoundValue);

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

