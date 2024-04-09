// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class COLOURSHOOTERGAME_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner")
	class UBoxComponent* SpawnVolume;

	UPROPERTY(EditAnywhere, Category = "Spawner")
	float SpawnDelayRangeLow;

	UPROPERTY(EditAnywhere, Category = "Spawner")
	float SpawnDelayRangeHigh;

	UPROPERTY(EditAnywhere, Category = "Spawner")
	float DifficultyScalar;

	UPROPERTY(EditAnywhere, Category = "Class to Spawn")
	TSubclassOf<class AEnemyCharacter> EnemyRedBlueprint;

	UPROPERTY(EditAnywhere, Category = "Class to Spawn")
	TSubclassOf<class AEnemyCharacter> EnemyBlueBlueprint;

	void SpawnEnemy();

	FTimerHandle SpawnTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FVector GetRandomPointInVolume();
	void StartSpawnTimer();

};
