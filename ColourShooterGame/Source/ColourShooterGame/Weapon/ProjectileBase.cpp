// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "Engine/DamageEvents.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Default projectile parameters.
	Damage = 10.0f;
	Speed = 1200.0f;
	LifeTime = 1.0f;
}

void AProjectileBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AActor::OnActorHit.AddDynamic(this, &AProjectileBase::OnActorHit);
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &AProjectileBase::DestroySelf, LifeTime, false);
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Location = GetActorLocation();
	Location += GetActorForwardVector() * Speed * DeltaTime;
	SetActorLocation(Location, true);
}

void AProjectileBase::OnActorHit(AActor* Self, AActor* Other, FVector NormalImpulse, const FHitResult& Hit)
{
	PrintMessageOnScreen("Hit actor");

	if (Other != nullptr)
	{
		PrintMessageOnScreen("Adding health");
		FDamageEvent DamageEvent;
		Other->TakeDamage(Damage, DamageEvent, nullptr, this);
	}

	// Destroy self.
	Destroy();
}

void AProjectileBase::DestroySelf()
{
	Destroy();
}

void AProjectileBase::PrintMessageOnScreen(FString Message)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(2, 10.f, FColor::Red, Message);
	}
}