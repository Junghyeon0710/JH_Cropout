// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/JHMainMenuGM.h"
#include "CommonActivatableWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameMode/JHGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "ShowLoginUICallbackProxy.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSubsystemTypes.h"
#include "AudioModulationStatics.h"
#include "UI/Game/Layer_Game_ActivatableWidget.h"
#include "UI/MainMenu/Layer_Menu_ActivatableWidget.h"
#include "UI/MainMenu/MainMenuActivatableWidget.h"


AJHMainMenuGM::AJHMainMenuGM()
{
	GameSessionClass = nullptr;
	
}

void AJHMainMenuGM::SpawningComplete()
{
}

void AJHMainMenuGM::BeginPlay()
{
	Super::BeginPlay();
	
	checkf(LayerMenuClass,TEXT("No LayerMenuClass"));

	
	ULayer_Menu_ActivatableWidget*  LayerMenu = CreateWidget<ULayer_Menu_ActivatableWidget>(UGameplayStatics::GetPlayerController(this,0),LayerMenuClass);
	LayerMenu->AddToViewport();
	//LayerMenu->OnActivate();
	LayerMenu->ActivateWidget(); //마우스 인풋이 안먹는 버그 있음,,;
	
	if (const UJHGameInstance* GameInstance =  Cast<UJHGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		GameInstance->TransitionOut();
	}

	checkf(RenderTarget2D,TEXT("No RenderTarget2D"));
	UKismetRenderingLibrary::ClearRenderTarget2D(this,RenderTarget2D,FLinearColor(0,0,0,1.f));

	
	// Online Subsystem 가져오기
	UShowLoginUICallbackProxy* ShowLoginUICallbackProxy = NewObject<UShowLoginUICallbackProxy>();


	//Mobile Only
	// if (ShowLoginUICallbackProxy)
	// {
	// 	ShowLoginUICallbackProxy->ShowExternalLoginUI(GetWorld(),UGameplayStatics::GetPlayerController(GetWorld(),0));
	// }
	
	//UShowLoginUICallbackProxy* LoginUIProxy = UShowLoginUICallbackProxy::ShowExternalLoginUI(GetWorld(),UGameplayStatics::GetPlayerController(GetWorld(),0));

	// if (LoginUIProxy)
	// {
	// 	LoginUIProxy->OnSuccess.AddDynamic(this,&ThisClass::OnLoginUISuccess);
	// }

	//Set Up Music Controls
	
	
	UAudioModulationStatics::SetGlobalBusMixValue(this,Cropout_Music_Piano_Vol,0.f,1.f);
	UAudioModulationStatics::SetGlobalBusMixValue(this,Cropout_Music_Perc_Vol,0.f,1.f);
	UAudioModulationStatics::SetGlobalBusMixValue(this,Cropout_Music_Strings_Delay,0.5f,1.f);
	UAudioModulationStatics::SetGlobalBusMixValue(this,Cropout_Music_WinLose,0.5f,1.f);
	
	if (UJHGameInstance* GI = Cast<UJHGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		GI->PlayMusic(Audio,1.f,true);
	}
}

void AJHMainMenuGM::OnLoginUISuccess(APlayerController* PlayerController)
{
	UE_LOG(LogTemp,Display,TEXT("Show login success"));
	UE_LOG(LogTemp,Display,TEXT("%s"),*PlayerController->GetName());
}
