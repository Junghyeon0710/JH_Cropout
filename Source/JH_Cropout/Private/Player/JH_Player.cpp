// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/JH_Player.h"


AJH_Player::AJH_Player()
{
 
	PrimaryActorTick.bCanEverTick = true;

	
}

void AJH_Player::BeginPlay()
{
	Super::BeginPlay();
	
}

void AJH_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

