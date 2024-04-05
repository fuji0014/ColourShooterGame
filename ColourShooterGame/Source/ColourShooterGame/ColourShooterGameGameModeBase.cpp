// Copyright Epic Games, Inc. All Rights Reserved.


#include "ColourShooterGameGameModeBase.h"
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
