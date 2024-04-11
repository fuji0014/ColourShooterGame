// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "../Pawn/MainCharacter.h"
#include "../UI/PlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"

// Sets default values
AMainPlayerController::AMainPlayerController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Begin play controller"));
	//UI HUD
	if (WBP_PlayerHUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("Begin play HUD"));
		MyHud = CreateWidget<UPlayerHUD>(GetWorld(), WBP_PlayerHUD);
		MyHud->AddToViewport();
	}

	bEnableClickEvents = true;
	bShowMouseCursor = true;
}

void AMainPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	TheMainChar = Cast<AMainCharacter>(P);

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

	if (InputComponent)
	{
		//UI HUD
		InputComponent->BindAction("ShowRestartButtonOnHUD", IE_Pressed, this, &AMainPlayerController::ShowRestartButtonOnHud);
	}
}

//UI HUD
void AMainPlayerController::ShowRestartButtonOnHud()
{
	if (MyHud)
	{
		MyHud->ShowLostScreen();
	}
}

