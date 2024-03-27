// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPawn.h"
#include "../Weapon/WeaponBase.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "UObject/UObjectGlobals.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/EngineBaseTypes.h"
#include "Components/SphereComponent.h"

#include "Math/UnrealMathUtility.h"
#include "Components/ArrowComponent.h"

// Sets default values
AMainPawn::AMainPawn()
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

	/*bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;*/

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	bGenerateOverlapEventsDuringLevelStreaming = true;

	VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	VisualMesh->SetCollisionProfileName(TEXT("IgnoreAll"));

	WeaponMount = CreateDefaultSubobject<UArrowComponent>("Weapon Mount");
	WeaponMount->SetupAttachment(GetRootComponent());

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;
}

void AMainPawn::MoveForward(float Amount)
{
	FloatingPawnMovement->AddInputVector(GetActorForwardVector() * Amount);
}

void AMainPawn::MoveRight(float Amount)
{
	FloatingPawnMovement->AddInputVector(GetActorRightVector() * Amount);
}

void AMainPawn::Turn(float Amount)
{
	AddControllerYawInput(Amount);
}

// Called when the game starts or when spawned
void AMainPawn::BeginPlay()
{
	Super::BeginPlay();
	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called every frame
void AMainPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainPawn::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AMainPawn::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainPawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Pickup", IE_Pressed, this, &AMainPawn::Interact);
	PlayerInputComponent->BindAction("Drop", IE_Pressed, this, &AMainPawn::DropWeapon);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AMainPawn::FirePressed);
	InputComponent->BindAction("Fire", IE_Released, this, &AMainPawn::FireReleased);

	InputComponent->BindAction("DecreaseHealth", IE_Released, this, &AMainPawn::DecreaseHealth);
}

void AMainPawn::Interact()
{
	//IF InteractSphere is null
	if (!InteractSphere)
	{
		InteractSphere = NewObject<USphereComponent>(this, USphereComponent::StaticClass());
		InteractSphere->RegisterComponent();
		InteractSphere->SetRelativeLocation(this->GetActorLocation());
		InteractSphere->SetSphereRadius(52.309566f);

		InteractSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		InteractSphere->SetGenerateOverlapEvents(true);

		DrawDebugSphere(GetWorld(), GetActorLocation(), InteractSphere->GetUnscaledSphereRadius(), 26, FColor(181, 0, 0), false, 2, 0, 2);
	}
	else
	{
		InteractSphere->DestroyComponent();
		InteractSphere = nullptr;
		Interact();
	}

	TArray<AActor*> items;
	InteractSphere->GetOverlappingActors(items, AItemBase::StaticClass());

	if (items.Num() > 0)
	{
		AItemBase* Item = Cast<AItemBase>(items[0]);
		if (Item == nullptr)
			return;

		AWeaponBase* Weapon = Cast<AWeaponBase>(Item);
		if (Weapon != nullptr)
		{
			HoldWeapon(Weapon);
		}
	}
}

void AMainPawn::HoldWeapon(AWeaponBase* Weapon)
{
	DropWeapon();

	CurrentWeapon = Weapon;
	CurrentWeapon->Attach(this);

	CurrentWeapon->OnWeaponFired.Clear();
	CurrentWeapon->OnWeaponFired.AddDynamic(this, &AMainPawn::OnWeaponFired);
}

void AMainPawn::DropWeapon()
{
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->OnWeaponFired.RemoveDynamic(this, &AMainPawn::OnWeaponFired);

		CurrentWeapon->Detach();
		CurrentWeapon = nullptr;

		bIsFiring = false;
	}
}

//WEEK8
void AMainPawn::FirePressed()
{
	Fire(true);
}

//WEEK8
void AMainPawn::FireReleased()
{
	Fire(false);
}

//WEEK8
void AMainPawn::Fire(bool Toggle)
{
	if (CurrentWeapon)
	{
		if (Toggle)
		{
			CurrentWeapon->PullTrigger();
		}
		else
		{
			CurrentWeapon->ReleaseTrigger();
		}

		bIsFiring = Toggle;
	}
}

void AMainPawn::DecreaseHealth()
{
	Health -= 10;
}

void AMainPawn::OnWeaponFired()
{
	FString Message = FString("OnWeaponFired: ");
	PrintMessageOnScreen(Message);
}

void AMainPawn::PrintMessageOnScreen(FString Message)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(2, 10.f, FColor::Red, Message);
	}
}
