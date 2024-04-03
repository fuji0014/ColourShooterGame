// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "../Pawn/MainPawn.h"

// Sets default values
AMainPlayerController::AMainPlayerController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMainPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	TheMainPawn = Cast<AMainPawn>(P);

}

void AMainPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	//TheMainPawn = Cast<AMainPawn>(aPawn); //NOPE
}

void AMainPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

// Called to bind functionality to input
void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
}
