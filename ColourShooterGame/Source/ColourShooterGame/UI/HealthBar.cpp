// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"
#include "Components/ProgressBar.h"
#include "../Pawn/MainPawn.h"
#include "Kismet/GameplayStatics.h"

bool UHealthBar::Initialize()
{
	bool Success = Super::Initialize();
	UE_LOG(LogTemp, Warning, TEXT("HealthBar Initialize"));
	if (HealthBar)
	{
		MainPawn = Cast<AMainPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainPawn::StaticClass()));
		HealthBar->SetPercent(100);
		if(MainPawn)
			HealthBar->PercentDelegate.BindUFunction(this, "UpdateHealthBar");
	}
	return true;
}

void UHealthBar::SetHealthPercentage(float Percentage)
{
	UE_LOG(LogTemp, Warning, TEXT("SetHealthPercentage"));
	if (HealthBar)
	{
		HealthBar->SetPercent(Percentage);
	}
}
