// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainSpawner.generated.h"

UCLASS()
class COLOURSHOOTERGAME_API AMainSpawner : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainSpawner();

	UPROPERTY(EditAnywhere, Category = "Class to Spawn")
	TSubclassOf<class AMainCharacter> MainCharacterBP;

	void SpawnCharacter();

	UPROPERTY(Transient)
	class AMainCharacter* ChararcterObject;

	UPROPERTY(Transient)
	bool bIsCharacterActive = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnCharacterDestroyed(AActor* DestroyedActor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
