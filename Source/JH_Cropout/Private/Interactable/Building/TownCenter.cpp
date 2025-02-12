// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/Building/TownCenter.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ATownCenter::ATownCenter()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("NavBlocker"));
	BoxComponent->SetupAttachment(Mesh);
}

void ATownCenter::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer,FTimerDelegate::CreateLambda([this]()
	{
		if(APlayerController* PC =  UGameplayStatics::GetPlayerController(this,0))
		{
			if(PC->GetPawn())
			{
				PC->GetPawn()->SetActorLocation(GetActorLocation());
			}
		}
	}), 1.f,false);
}

void ATownCenter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	Tags.AddUnique(FName("All Resources"));
}
