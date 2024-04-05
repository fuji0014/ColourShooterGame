// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnemyCharacter.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create the spawn volume box component
    SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
    SetRootComponent(SpawnVolume);

    // Set default values for spawn delay range and difficulty scalar
    SpawnDelayRangeLow = 1.0f;
    SpawnDelayRangeHigh = 2.0f;
    DifficultyScalar = 0.5f;
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
    StartSpawnTimer();
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AEnemySpawner::SpawnEnemy()
{
    // Calculate new spawn delays based on difficulty scalar
    SpawnDelayRangeLow /= DifficultyScalar;
    SpawnDelayRangeHigh /= DifficultyScalar;

    // Calculate spawn delay
    float SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);

    FVector SpawnLocation = GetRandomPointInVolume();
    AEnemyCharacter* NewEnemy = GetWorld()->SpawnActor<AEnemyCharacter>(EnemyBlueprint, SpawnLocation, FRotator::ZeroRotator);

    // Set timer for the next spawn
    GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemySpawner::SpawnEnemy, SpawnDelay, false);
}

FVector AEnemySpawner::GetRandomPointInVolume()
{
    FVector SpawnOrigin = SpawnVolume->Bounds.Origin;
    FVector SpawnExtent = SpawnVolume->Bounds.BoxExtent;
    return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent);
}

void AEnemySpawner::StartSpawnTimer()
{
    // Initial spawn delay
    SpawnEnemy();
}