// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ColourShooterGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class COLOURSHOOTERGAME_API AColourShooterGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool GameStarted = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool SpawnerPaused = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
