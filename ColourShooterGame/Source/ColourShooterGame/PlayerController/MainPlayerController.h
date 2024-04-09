// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

UCLASS()
class COLOURSHOOTERGAME_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainPlayerController();

	virtual void AcknowledgePossession(class APawn* P) override;

	//UI HUD
	UPROPERTY(EditAnywhere, Category = HUD)
	TSubclassOf<class UPlayerHUD> WBP_PlayerHUD;

	//UI HUD
	class UPlayerHUD* MyHud;

protected:
	virtual void BeginPlay() override;

	class UFloatingPawnMovement* FloatingPawnMovement;

	float CurrentVelocity = 0;

	//UI HUD
	void ShowRestartButtonOnHud();

public:	
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

	virtual void SetupInputComponent() override;

	class AMainCharacter* TheMainChar;
};
