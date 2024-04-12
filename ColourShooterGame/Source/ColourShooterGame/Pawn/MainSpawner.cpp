// Fill out your copyright notice in the Description page of Project Settings.


#include "MainSpawner.h"
#include "MainCharacter.h"

// Sets default values
AMainSpawner::AMainSpawner()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMainSpawner::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMainSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainSpawner::SpawnCharacter()
{
	if (!IsValid(ChararcterObject))
	{
		if (bIsCharacterActive == false)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			ChararcterObject = GetWorld()->SpawnActor<AMainCharacter>(MainCharacterBP, GetActorLocation(), GetActorRotation(), SpawnParams);

			if (ChararcterObject)
			{
				ChararcterObject->OnDestroyed.AddDynamic(this, &AMainSpawner::OnCharacterDestroyed);
				bIsCharacterActive = true;
			}
		}
	}
}

void AMainSpawner::OnCharacterDestroyed(AActor* DestroyedActor)
{
	bIsCharacterActive = false;
	DestroyedActor->OnDestroyed.RemoveAll(this);
}
