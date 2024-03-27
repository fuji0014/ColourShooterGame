// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include <Components/SkeletalMeshComponent.h>
#include <Components/PrimitiveComponent.h>
#include <Components/ArrowComponent.h>
#include <Components/SphereComponent.h>
#include "ProjectileBase.h"

#include "../Pawn/MainPawn.h"

AWeaponBase::AWeaponBase() : Primitive(nullptr),	Muzzle(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	FireRate = 10.0f;

	MaximumAmmo = 200;
	CurrentAmmo = -1;
}

void AWeaponBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Primitive = Cast<UPrimitiveComponent>(GetRootComponent());

	auto Components = GetComponents();
	for (auto Component : Components)
	{
		if (Component->GetFName() == "Muzzle")
		{
			Muzzle = Cast<UArrowComponent>(Component);
		}

		if (Component->IsA<USphereComponent>())
			CollsionSphere = Cast<USphereComponent>(Component);
	}
	MaximumAmmo = FMath::Max(1, MaximumAmmo);
	CurrentAmmo = FMath::Min(CurrentAmmo, MaximumAmmo);

	if (CurrentAmmo < 0)
	{
		CurrentAmmo = MaximumAmmo;
	}
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponBase::Attach(class APawn* Character)
{
	verify(Character != nullptr && "Weapon's attach function called with null character!");

	SetOwner(Character);
	SetInstigator(Character);

	//SetActorEnableCollision(false);
	//Primitive->SetSimulatePhysics(false);
	CollsionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);//  SetActorEnableCollision(false);

	// Attach weapon to the character's mesh.
	//CALL AttachToComponent() and pass in (Character->GetSkeletalMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "WeaponSocket") <-- We are attaching this Actor to the Characters Skeletal Mesh at the WeaponSocket
	//AttachToComponent(Character->GetSkeletalMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "WeaponSocket");
	//AttachToComponent(Cast<AMainPawn>(Character)->WeaponMount, FAttachmentTransformRules::SnapToTargetIncludingScale);
	AttachToComponent(Cast<AMainPawn>(Character)->VisualMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "MountPoint1");
	//AttachToComponent(Cast<AMainPawn>(Character)->VisualMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, "MountPoint2");
}

void AWeaponBase::Detach()
{
	ReleaseTrigger();

	SetOwner(NULL);

	//SetActorEnableCollision(true);
	//Primitive->SetSimulatePhysics(true);
	CollsionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void AWeaponBase::PullTrigger()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (CurrentAmmo > 0)
	{
		float RemainingTime = FMath::Max(TimerManager.GetTimerRemaining(FireTimer), 0.0f);
		TimerManager.SetTimer(FireTimer, this, &AWeaponBase::Fire, 1.0f / FireRate, true, RemainingTime);
	}
}

void AWeaponBase::ReleaseTrigger()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (TimerManager.TimerExists(FireTimer))
	{
		float RemainingTime = TimerManager.GetTimerRemaining(FireTimer);
		TimerManager.SetTimer(FireTimer, this, &AWeaponBase::ClearFireTimer, RemainingTime, false);
	}
}

void AWeaponBase::Fire()
{
	if (CurrentAmmo > 0)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = Cast<APawn>(GetOwner());
		SpawnParams.Owner = this;

		FTransform Transform = Muzzle->GetComponentToWorld();
		GetWorld()->SpawnActor<AActor>(ProjectileType, Transform.GetLocation(), Transform.GetRotation().Rotator(), SpawnParams);

		OnWeaponFired.Broadcast();

		CurrentAmmo--;
	}
}

void AWeaponBase::ClearFireTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(FireTimer);
}

FVector AWeaponBase::GetMuzzleLocation() const
{
	return Muzzle->GetComponentToWorld().GetLocation();
}