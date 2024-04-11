// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "MainCharacter.h"
#include "NavigationSystem.h"
#include "Perception/PawnSensingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemy::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AEnemy::OnNoiseHeard);

	GuardState = EAIState::Idle;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	OriginalRotator = GetActorRotation();
	MoveToNextPatrolPoint();

	OnActorHit.AddDynamic(this, &AEnemy::OnHitActor);
}

void AEnemy::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr)
	{
		return;
	}

	TargetActor = SeenPawn;
	//DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);
	UE_LOG(LogTemp, Warning, TEXT("Seen"));

	SetGuardState(EAIState::Alerted);

	AController* PawnController = GetController();
	if (PawnController)
	{
		PawnController->StopMovement();
		GetWorldTimerManager().ClearTimer(TimerHandle_ResetState);
		GetWorldTimerManager().SetTimer(TimerHandle_ResetState, this, &AEnemy::ResetPatrol, 3.0f);
	}
}

void AEnemy::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}

	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f);
	UE_LOG(LogTemp, Warning, TEXT("heard"));

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();
	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator(); //Can use the one in kismet library (LookAt()) X is to look forward dir
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f;
	SetActorRotation(NewLookAt);
	GetWorldTimerManager().ClearTimer(TimerHandle_ResetState);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetState, this, &AEnemy::ResetState, 3.0f);

	SetGuardState(EAIState::Suspicious);

	AController* PawnController = GetController();
	if (PawnController)
	{
		PawnController->StopMovement();
	}
}

void AEnemy::ResetState()
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}

	SetActorRotation(OriginalRotator);
	ResetPatrol();
}


void AEnemy::ResetPatrol()
{
	TargetActor = nullptr;
	SetGuardState(EAIState::Idle);

	if (bPatrol)
	{
		MoveToNextPatrolPoint();
	}
}

void AEnemy::SetGuardState(EAIState NewState)
{if (GuardState == NewState)
	{
		return;
	}

	GuardState = NewState;
}

void AEnemy::MoveToNextPatrolPoint()
{
	if (CurrentPatrolPoint == nullptr || CurrentPatrolPoint == SecondPatrolPoint)
	{
		CurrentPatrolPoint = FirstPatrolPoint;
	}
	else
	{
		CurrentPatrolPoint = SecondPatrolPoint;
	}

	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentPatrolPoint);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentPatrolPoint)
	{
		FVector Delta = GetActorLocation() - CurrentPatrolPoint->GetActorLocation();

		float DistanceToGoal = Delta.Size();

		UE_LOG(LogTemp, Warning, TEXT("DistanceToGoal = %f"), DistanceToGoal);

		if (DistanceToGoal < 150)
		{
			MoveToNextPatrolPoint();
		}
	}

	if (TargetActor)
	{
		FVector MyLoc = GetActorLocation();
		FVector TargetLoc = TargetActor->GetActorLocation();
		FVector Dir = (TargetLoc - MyLoc);
		Dir.Normalize();
		SetActorRotation(FMath::Lerp(GetActorRotation(), Dir.Rotation(), 0.01f));

		if (GuardState == EAIState::Alerted)
		{
			UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), TargetActor);
		}
	}
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::DecreaseHealth()
{
	CurrentHealth -= 20;
	if (CurrentHealth <= 0) {
		PrintMessageOnScreen("You killed an enemy.");
		Destroy();
	}

}

void AEnemy::PrintMessageOnScreen(FString Message)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(2, 10.f, FColor::Red, Message);
	}
}

void AEnemy::OnHitActor(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
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
