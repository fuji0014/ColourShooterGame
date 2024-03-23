// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
AMainPlayerController::AMainPlayerController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	BoxComponent->SetSimulatePhysics(true);
	BoxComponent->SetCollisionProfileName("Pawn");
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetRootComponent(BoxComponent);
	SpringArm->SetupAttachment(RootComponent);
	VisualMesh->SetupAttachment(RootComponent);
	MainCamera->SetupAttachment(SpringArm);

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	VisualMesh->SetCollisionProfileName(TEXT("IgnoreAll"));
}

void AMainPlayerController::MoveForward(float Amount)
{
	FloatingPawnMovement->AddInputVector(GetActorForwardVector() * Amount);
}

void AMainPlayerController::MoveRight(float Amount)
{
	FloatingPawnMovement->AddInputVector(GetActorRightVector() * Amount);
}

void AMainPlayerController::Turn(float Amount)
{
	AddControllerYawInput(Amount);
}

// Called when the game starts or when spawned
void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainPlayerController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainPlayerController::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainPlayerController::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AMainPlayerController::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainPlayerController::AddControllerPitchInput);
}

