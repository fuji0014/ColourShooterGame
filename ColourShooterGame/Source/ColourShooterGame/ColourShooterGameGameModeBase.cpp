// Copyright Epic Games, Inc. All Rights Reserved.


#include "ColourShooterGameGameModeBase.h"
#include "PlayerController/MainPlayerController.h"
#include "Pawn/MainCharacter.h"
#include "UI/PlayerHUD.h"
#include "Kismet/GameplayStatics.h"

AColourShooterGameGameModeBase::AColourShooterGameGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AColourShooterGameGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	DefaultPawnClass = AMainPlayerController::StaticClass();
}

void AColourShooterGameGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

APawn* AColourShooterGameGameModeBase::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
    // Spawn the default pawn at the PlayerStart location
    if (DefaultPawnClass != nullptr)
    {
        return SpawnDefaultPawnAtTransform(NewPlayer, StartSpot->GetActorTransform());
    }

    return nullptr;
}
