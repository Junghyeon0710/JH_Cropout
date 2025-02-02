// Fill out your copyright notice in the Description page of Project Settings.


#include "Extras/Cheats.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameMode/ResourceInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"

UCheats::UCheats()
{

	PrimaryComponentTick.bCanEverTick = false;


}


// Called when the game starts
void UCheats::BeginPlay()
{
	Super::BeginPlay();

	const ULocalPlayer* LocalPlayer =  UGameplayStatics::GetPlayerController(this,0)->GetLocalPlayer();
	check(LocalPlayer);
	if (UEnhancedInputLocalPlayerSubsystem* LS = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
	{
		LS->AddMappingContext(CheatInputMappingContext,1);
	}
	
	AActor* Owner = GetOwner();
	if (Owner)
	{
		// Owner의 InputComponent 가져오기
		UInputComponent* InputComp = Owner->FindComponentByClass<UInputComponent>();
		if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComp))
		{
			EnhancedInputComponent->BindAction(Key1Action, ETriggerEvent::Started, this, &UCheats::Key1Event);
			EnhancedInputComponent->BindAction(Key2Action, ETriggerEvent::Started, this, &UCheats::Key2Event);
			EnhancedInputComponent->BindAction(Key3Action, ETriggerEvent::Started, this, &UCheats::Key3Event);
		}
	}
}

void UCheats::Key1Event()
{
	GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,TEXT("Key1Input"));
	if(IResourceInterface* RIF = Cast<IResourceInterface>( UGameplayStatics::GetGameMode(this)))
	{
		RIF->AddResource(EResourceType::Food, 15);
	}
}

void UCheats::Key2Event()
{
	if(IResourceInterface* RIF = Cast<IResourceInterface>( UGameplayStatics::GetGameMode(this)))
	{
		RIF->AddResource(EResourceType::Wood, 15);
	}
}

void UCheats::Key3Event()
{
	if(IResourceInterface* RIF = Cast<IResourceInterface>( UGameplayStatics::GetGameMode(this)))
	{
		RIF->AddResource(EResourceType::Stone, 15);
	}
}



