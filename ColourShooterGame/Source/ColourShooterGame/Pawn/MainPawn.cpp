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
#include "Components/WidgetComponent.h"
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

	WeaponMountRight = CreateDefaultSubobject<UArrowComponent>("RightWeaponMount");
	WeaponMountLeft = CreateDefaultSubobject<UArrowComponent>("LeftWeaponMount");

	WeaponMountRight->SetupAttachment(GetRootComponent());
	WeaponMountLeft->SetupAttachment(GetRootComponent());

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;

	//MainCamera->bUsePawnControlRotation = true;

	BoxComponent->GetBodyInstance()->bLockZTranslation = true;

	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>("Health Bar");
	HealthBarComponent->SetupAttachment(RootComponent);

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
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMainPawn::OnCollisionBoxBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AMainPawn::OnCollisionBoxEndOverlap);

	BoxComponent->OnComponentHit.AddDynamic(this, &AMainPawn::OnCollisionBoxHit);

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
	//PlayerInputComponent->BindAxis("LookUp", this, &AMainPawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Pickup", IE_Pressed, this, &AMainPawn::Interact);
	PlayerInputComponent->BindAction("Drop", IE_Pressed, this, &AMainPawn::DropWeapon);

	InputComponent->BindAction("FireRight", IE_Pressed, this, &AMainPawn::FireRightPressed);
	InputComponent->BindAction("FireRight", IE_Released, this, &AMainPawn::FireRightReleased);
	InputComponent->BindAction("FireLeft", IE_Pressed, this, &AMainPawn::FireLeftPressed);
	InputComponent->BindAction("FireLeft", IE_Released, this, &AMainPawn::FireLeftReleased);

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
	//DropWeapon();
	if (Weapon->weaponPosition == EWeaponType::RightWeapon) {
		CurrentRightWeapon = Weapon;
		CurrentRightWeapon->Attach(this);

		CurrentRightWeapon->OnWeaponFired.Clear();
		CurrentRightWeapon->OnWeaponFired.AddDynamic(this, &AMainPawn::OnWeaponFired);
	}
	else {
		CurrentLeftWeapon = Weapon;
		CurrentLeftWeapon->Attach(this);

		CurrentLeftWeapon->OnWeaponFired.Clear();
		CurrentLeftWeapon->OnWeaponFired.AddDynamic(this, &AMainPawn::OnWeaponFired);
	}
}

void AMainPawn::DropWeapon()
{
	/*if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->OnWeaponFired.RemoveDynamic(this, &AMainPawn::OnWeaponFired);

		CurrentWeapon->Detach();
		CurrentWeapon = nullptr;

		bIsFiring = false;
	}*/
}

void AMainPawn::FireRightPressed()
{
	FireRight(true);
}

void AMainPawn::FireRightReleased()
{
	FireRight(false);
}

void AMainPawn::FireLeftPressed()
{
	FireLeft(true);
}

void AMainPawn::FireLeftReleased()
{
	FireLeft(false);
}

void AMainPawn::FireRight(bool Toggle)
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

void AMainPawn::FireLeft(bool Toggle)
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

void AMainPawn::OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AMainPawn::OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AMainPawn::OnCollisionBoxHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		FString Message = FString("OnCollisionBoxHit: ") + OtherActor->GetName();
		PrintMessageOnScreen(Message);
	}
}