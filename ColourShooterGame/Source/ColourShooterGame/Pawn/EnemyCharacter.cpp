// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));

	CollisionSphere->SetSimulatePhysics(true);
	CollisionSphere->SetCollisionProfileName("BlockAllDynamic");
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetRootComponent(CollisionSphere);
	VisualMesh->SetupAttachment(RootComponent);

	VisualMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	VisualMesh->SetCollisionProfileName(TEXT("IgnoreAll"));

	CollisionSphere->GetBodyInstance()->bLockZTranslation = true;

}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Spawned!"));
	OnActorHit.AddDynamic(this, &AEnemyCharacter::OnHitActor);

}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MainChar = Cast<AMainCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainCharacter::StaticClass()));

	if (MainChar != NULL)
	{
		FVector EnemyLocation = GetActorLocation();
		FVector TargetLocation = MainChar->GetActorLocation();
		//TargetLocation.X = MainPawn->GetActorLocation().X;
		//TargetLocation.Y = MainPawn->GetActorLocation().Y;
		FVector NewLocation = FMath::Lerp(EnemyLocation, TargetLocation, 0.01f);
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

void AEnemyCharacter::DecreaseHealth()
{
	Health -= 20;
	if (Health <= 0) {
		PrintMessageOnScreen("You killed an enemy.");
		Destroy();
	}
		
}

void AEnemyCharacter::OnHitActor(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("hit actor!"));
	if (OtherActor && OtherActor != this && OtherActor->IsA<AMainCharacter>())
	{
		AMainCharacter* HitMainChar = Cast<AMainCharacter>(OtherActor);
		if (HitMainChar)
		{
			HitMainChar->DecreaseHealth();
		}
	}
}


void AEnemyCharacter::OnCollisionSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		FString Message = FString("OnCollisionSphereHit: ") + OtherActor->GetName();
		PrintMessageOnScreen(Message);
	}
}