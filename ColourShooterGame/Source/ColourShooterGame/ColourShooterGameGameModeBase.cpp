// Copyright Epic Games, Inc. All Rights Reserved.


#include "ColourShooterGameGameModeBase.h"
#include "PlayerController/MainPlayerController.h"
#include "UI/PlayerHUD.h"
#include "Kismet/GameplayStatics.h"

AColourShooterGameGameModeBase::AColourShooterGameGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AColourShooterGameGameModeBase::BeginPlay()
{
	Super::BeginPlay();

}

void AColourShooterGameGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
