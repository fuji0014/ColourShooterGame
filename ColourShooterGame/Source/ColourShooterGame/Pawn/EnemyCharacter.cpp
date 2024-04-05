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

	OnActorHit.AddDynamic(this, &AEnemyCharacter::OnHitActor);
	//CollisionSphere->OnComponentHit.AddDynamic(this, &AEnemyCharacter::OnCollisionSphereHit);

	if (VisualMesh)
	{
		// Create a dynamic material instance from the VisualMesh's material
		UMaterialInstanceDynamic* DynamicMaterial = VisualMesh->CreateAndSetMaterialInstanceDynamic(0);
		if (DynamicMaterial)
		{
			
			// Randomly choose between 0 and 1
			int32 RandomIndex = FMath::RandBool() ? 1 : 0;
			EnemyType = (RandomIndex == 0) ? EEnemyType::RedEnemy : EEnemyType::BlueEnemy;

			FLinearColor ChosenColor = (RandomIndex == 0) ? FLinearColor(1.0f, 0.0f, 0.0f) : FLinearColor(0.0f, 0.0f, 1.0f);
			UE_LOG(LogTemp, Warning, TEXT("Chosen color: %s"), *ChosenColor.ToString());

			DynamicMaterial->SetVectorParameterValue("BaseColor", ChosenColor);
			VisualMesh->SetMaterial(0, DynamicMaterial);
		}
	}
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MainPawn = Cast<AMainPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainPawn::StaticClass()));

	if (MainPawn != NULL)
	{
		FVector EnemyLocation = GetActorLocation();
		FVector TargetLocation = MainPawn->GetActorLocation();
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
	if (OtherActor && OtherActor != this && OtherActor->IsA<AMainPawn>()) 
	{
		AMainPawn* HitMainPawn = Cast<AMainPawn>(OtherActor);
		if (HitMainPawn)
		{
			HitMainPawn->DecreaseHealth();
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