// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "../PlayerController/MainPlayerController.h"
#include "../Weapon/WeaponBase.h"
#include "../UI/HealthBar.h"
#include "../UI/PlayerHUD.h"

#include <GameFramework/PlayerController.h>

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/ArrowComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMainCharacter::AMainCharacter() :
	SkeletalMesh(nullptr),
	AnimationInstance(nullptr)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMountRight = CreateDefaultSubobject<UArrowComponent>("RightWeaponMount");
	WeaponMountLeft = CreateDefaultSubobject<UArrowComponent>("LeftWeaponMount");

	WeaponMountRight->SetupAttachment(GetRootComponent());
	WeaponMountLeft->SetupAttachment(GetRootComponent());

	MaxWalkSpeed = 140.0f;
	MaxJogSpeed = 280.0f;

	//bUseControllerRotationYaw = false;

	//GetCharacterMovement()->bOrientRotationToMovement = true;

	MaxPickUpDistance = 200.0f;

	CurrentWeapon = nullptr;

	bIsFiring = false;

	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>("Health Bar");
	HealthBarComponent->SetupAttachment(RootComponent);
}
void AMainCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SkeletalMesh = GetMesh();


	USpringArmComponent* SpringArm = FindComponentByClass<USpringArmComponent>();

	if (SpringArm)
	{
		SpringArm->bDoCollisionTest = false;
		//SpringArm->bInheritPitch = false;
		//SpringArm->bInheritYaw = false;
		//SpringArm->bInheritRoll = false;
		//SpringArm->bEnableCameraLag = true;
		//SpringArm->CameraLagSpeed = 3.0f;
	}
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetCanBeDamaged(true);
	OnTakeAnyDamage.AddDynamic(this, &AMainCharacter::OnTakeDamage);
	CurrentHealth = MaximumHealth;

	/*if (WBP_PlayerHUD) 
	{
		Hud = CreateWidget<UPlayerHUD>(GetWorld(), WBP_PlayerHUD);
		Hud->AddToViewport();
	}*/
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveVertical);
	InputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveHorizontal);

	PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::Turn);
	PlayerInputComponent->BindAction("Pickup", IE_Pressed, this, &AMainCharacter::Interact);

	InputComponent->BindAction("FireRight", IE_Pressed, this, &AMainCharacter::FireRightPressed);
	InputComponent->BindAction("FireRight", IE_Released, this, &AMainCharacter::FireRightReleased);
	InputComponent->BindAction("FireLeft", IE_Pressed, this, &AMainCharacter::FireLeftPressed);
	InputComponent->BindAction("FireLeft", IE_Released, this, &AMainCharacter::FireLeftReleased);
	
}


// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = MaxJogSpeed;

	//UpdateMovement(DeltaTime);
}

void AMainCharacter::UpdateMovement(float DeltaTime)
{
	//
}

void AMainCharacter::MoveVertical(float Scale)
{
	if (bMovementEnabled) 
		Move(GetActorForwardVector(), Scale);
}

void AMainCharacter::MoveHorizontal(float Scale)
{
	if (bMovementEnabled)
		Move(GetActorRightVector(), Scale);
}

void AMainCharacter::Move(FVector Direction, float Scale)
{
	if (bMovementEnabled)
		AddMovementInput(Direction, Scale);
}

void AMainCharacter::FireRightPressed()
{
	if (bMovementEnabled)
		FireRight(true);
}

void AMainCharacter::FireRightReleased()
{
	if (bMovementEnabled)
		FireRight(false);
}

void AMainCharacter::FireLeftPressed()
{
	if (bMovementEnabled)
		FireLeft(true);
}

void AMainCharacter::FireLeftReleased()
{
	if (bMovementEnabled)
		FireLeft(false);
}

void AMainCharacter::FireRight(bool Toggle)
{
	if (CurrentRightWeapon)
	{
		if (Toggle)
		{
			CurrentRightWeapon->PullTrigger();
		}
		else
		{
			CurrentRightWeapon->ReleaseTrigger();
		}

		bIsFiring = Toggle;
	}
}

void AMainCharacter::FireLeft(bool Toggle)
{
	if (CurrentLeftWeapon)
	{
		if (Toggle)
		{
			CurrentLeftWeapon->PullTrigger();
		}
		else
		{
			CurrentLeftWeapon->ReleaseTrigger();
		}

		bIsFiring = Toggle;
	}
}

void AMainCharacter::Interact()
{
	if (!InteractSphere)
	{
		InteractSphere = NewObject<USphereComponent>(this, USphereComponent::StaticClass());
		InteractSphere->RegisterComponent();
		InteractSphere->SetRelativeLocation(this->GetActorLocation());
		InteractSphere->SetSphereRadius(52.309566f);

		InteractSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		InteractSphere->SetGenerateOverlapEvents(true);

		//DrawDebugSphere(GetWorld(), GetActorLocation(), InteractSphere->GetUnscaledSphereRadius(), 26, FColor(181, 0, 0), false, 2, 0, 2);
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

void AMainCharacter::HoldWeapon(AWeaponBase* Weapon)
{
	//DropWeapon();
	if (Weapon->weaponPosition == EWeaponType::RightBlueWeapon) {
		CurrentRightWeapon = Weapon;
		CurrentRightWeapon->Attach(this);

		CurrentRightWeapon->OnWeaponFired.Clear();
		CurrentRightWeapon->OnWeaponFired.AddDynamic(this, &AMainCharacter::OnWeaponFired);
	}
	else {
		CurrentLeftWeapon = Weapon;
		CurrentLeftWeapon->Attach(this);

		CurrentLeftWeapon->OnWeaponFired.Clear();
		CurrentLeftWeapon->OnWeaponFired.AddDynamic(this, &AMainCharacter::OnWeaponFired);
	}
}

void AMainCharacter::OnWeaponFired()
{
	UE_LOG(LogTemp, Warning, TEXT("Weapon fired"));
}

void AMainCharacter::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaximumHealth);
}


void AMainCharacter::DecreaseHealth()
{
	CurrentHealth -= 1;
	CurrentHealth = FMath::Max(CurrentHealth, 0.0f);
	if (CurrentHealth <= 0) {
		//Destroy();
		//CurrentHealth = 100;
		//Hud->ShowLostScreen();
		Destroy();
		AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetController());
		PlayerController->ShowRestartButtonOnHud();

		UE_LOG(LogTemp, Warning, TEXT("Lost screen triggered"));
		
	}

	if (HealthBarComponent)
	{
		UpdateHealthBar();
	}
}

void AMainCharacter::UpdateHealthBar()
{
	UE_LOG(LogTemp, Warning, TEXT("UpdateHealthBar"));

	if (HealthBarComponent) {
		UHealthBar* HealthBarWidget = Cast<UHealthBar>(HealthBarComponent->GetUserWidgetObject());
		if (HealthBarWidget)
		{
			float HealthPercentage = CurrentHealth / MaximumHealth;
			HealthBarWidget->SetHealthPercentage(HealthPercentage);
		}
	}
}

void AMainCharacter::Turn(float Scale)
{
	AddControllerYawInput(Scale);
}