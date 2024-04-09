// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBar.h"
#include "Components/ProgressBar.h"
#include "../Pawn/MainCharacter.h"
#include "Kismet/GameplayStatics.h"

bool UHealthBar::Initialize()
{
	bool Success = Super::Initialize();
	UE_LOG(LogTemp, Warning, TEXT("HealthBar Initialize"));
	if (HealthBar)
	{
		MainChar = Cast<AMainCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainCharacter::StaticClass()));
		HealthBar->SetPercent(100);
		if(MainChar)
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
