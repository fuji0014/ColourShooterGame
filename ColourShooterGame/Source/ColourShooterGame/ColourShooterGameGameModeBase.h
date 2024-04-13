// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ColourShooterGameGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class COLOURSHOOTERGAME_API AColourShooterGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "MyGameMode")
	TSubclassOf<class AMainCharacter> DefaultCharacterClass; // The default character class to spawn

	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;

public:
	AColourShooterGameGameModeBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
