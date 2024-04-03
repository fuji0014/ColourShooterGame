// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MainPawn.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));

	CollisionSphere->SetSimulatePhysics(true);
	CollisionSphere->SetCollisionProfileName("Pawn");
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetRootComponent(CollisionSphere);
	VisualMesh->SetupAttachment(RootComponent);

	VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	VisualMesh->SetCollisionProfileName(TEXT("IgnoreAll"));

	CollisionSphere->GetBodyInstance()->bLockZTranslation = true;

	CurrentHealth = 0;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MainPawn = Cast<AMainPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainPawn::StaticClass()));

	if (MainPawn != NULL)
	{
		FVector BallLocation = MainPawn->GetActorLocation();
		FVector TargetLocation = GetActorLocation();
		TargetLocation.X = MainPawn->GetActorLocation().X;
		TargetLocation.Y = MainPawn->GetActorLocation().Y;
		FVector NewLocation = FMath::Lerp(GetActorLocation(), TargetLocation, 0.01f);
		SetActorLocation(NewLocation);
	}
}

void AEnemyCharacter::PrintMessageOnScreen(FString Message)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(2, 10.f, FColor::Red, Message);
	}
}