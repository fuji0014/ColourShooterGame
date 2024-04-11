// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

#include "../Pawn/MainCharacter.h"
#include "../Pawn/EnemySpawner.h"
#include "../GameState/ColourShooterGameStateBase.h"
#include "../Weapon/WeaponBase.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/BackgroundBlur.h"
#include "Styling/SlateColor.h"
#include "Kismet/GameplayStatics.h"

//UI HUD
void UPlayerHUD::NativeConstruct()
{
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		AColourShooterGameStateBase* GameState = Cast<AColourShooterGameStateBase>(GetWorld()->GetGameState());
		if (GameState && !GameState->GameStarted)
		{
			PC->SetIgnoreMoveInput(true);
			PC->SetIgnoreLookInput(true);

			AEnemySpawner* Spawner = Cast<AEnemySpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemySpawner::StaticClass()));
			if(Spawner)
			{
				Spawner->PauseSpawning();
			}
		}
	}
	
	if (ClickMeButton)
	{
		ClickMeButton->SetVisibility(ESlateVisibility::Hidden);
		ClickMeButton->OnClicked.AddDynamic(this, &UPlayerHUD::ButtonClicked);
		YouLostText->SetVisibility(ESlateVisibility::Hidden);
	}


	if (StartButton)
	{
		StartButton->SetVisibility(ESlateVisibility::Visible);
		StartButton->OnClicked.AddDynamic(this, &UPlayerHUD::StartButtonClicked);
	}

	if (BackgroundBlur)
	{
		BackgroundBlur->SetBlurStrength(5.0f);
	}
}

//UI HUD
bool UPlayerHUD::Initialize()
{
	bool Success = Super::Initialize();
	if (WeaponAmmoCountText)
	{
		WeaponAmmoCountText->TextDelegate.BindUFunction(this, "SetWeaponAmmoCount");
	}

	if (HealthBar)
	{
		HealthBar->PercentDelegate.BindUFunction(this, "SetHealthProgress");
	}

	return true;
}

//UI HUD
FText UPlayerHUD::SetWeaponAmmoCount()
{
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		AMainCharacter* PlayerChar = Cast<AMainCharacter>(PC->GetPawn());
		if (PlayerChar)
		{
			AWeaponBase* WeaponRight = PlayerChar->CurrentRightWeapon;
			AWeaponBase* WeaponLeft = PlayerChar->CurrentLeftWeapon;
			//WeaponAmmoCountText->SetText(FText::AsNumber(Weapon->CurrentAmmo) );
			//return FText::FromString(FString::FromInt(Weapon->CurrentAmmo));
			int AmmoRightCount = 0;
			int AmmoLeftCount = 0;
			int MaxAmmo = 0;
			if (WeaponRight)
			{
				AmmoRightCount = WeaponRight->CurrentAmmo;
				MaxAmmo = WeaponRight->MaximumAmmo;
			}
			if (WeaponLeft)
			{
				AmmoLeftCount = WeaponLeft->CurrentAmmo;
				MaxAmmo = WeaponLeft->MaximumAmmo;
			}
			return FText::FromString("Right Ammo: " + FString::FromInt(AmmoRightCount) + "/" + FString::FromInt(MaxAmmo)
				+ "\nLeft Ammo:" + FString::FromInt(AmmoLeftCount) + "/" + FString::FromInt(MaxAmmo));
			//return FText::AsNumber(AmmoCount);
		}
	}
	return FText::FromString("NULL");
}

float UPlayerHUD::SetHealthProgress()
{

	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		AMainCharacter* PlayerChar = Cast<AMainCharacter>(PC->GetPawn());
		if (PlayerChar)
		{
			return PlayerChar->CurrentHealth / PlayerChar->MaximumHealth;
		}

	}
	return 1;
}

void UPlayerHUD::ShowLostScreen() 
{
	AColourShooterGameStateBase* GameState = GetWorld()->GetGameState<AColourShooterGameStateBase>();
	if (GameState)
	{
		GameState->GameStarted = false;
		GameState->SpawnerPaused = true;
	}

	AEnemySpawner* Spawner = Cast<AEnemySpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemySpawner::StaticClass()));
	if (Spawner)
	{
		Spawner->PauseSpawning();
	}

	AMainCharacter* MainChar = Cast<AMainCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainCharacter::StaticClass()));
	if (MainChar)
	{
		MainChar->bMovementEnabled = false;
	}

	if (BackgroundBlur)
	{
		BackgroundBlur->SetBlurStrength(5.0f);
	}

	if (ClickMeButton)
	{
		ClickMeButton->SetVisibility(ESlateVisibility::Visible);
		YouLostText->SetVisibility(ESlateVisibility::Visible);
	}
}

//UI HUD
void UPlayerHUD::ButtonClicked()
{

	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		PC->ConsoleCommand("RestartLevel");
	}

}

void UPlayerHUD::StartButtonClicked()
{
	
	// Start the game
	AColourShooterGameStateBase* GameState = Cast<AColourShooterGameStateBase>(GetWorld()->GetGameState());
	if (GameState)
	{
		GameState->GameStarted = true;
		GameState->SpawnerPaused = false;
	}

	AEnemySpawner* Spawner = Cast<AEnemySpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemySpawner::StaticClass()));
	if (Spawner)
	{
		Spawner->ResumeSpawning();
	}

	AMainCharacter* MainChar = Cast<AMainCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainCharacter::StaticClass()));
	if (MainChar)
	{
		MainChar->bMovementEnabled = true;
	}

	// Enable player input
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		PC->SetIgnoreMoveInput(false);
		PC->SetIgnoreLookInput(false);
	}

	if (BackgroundBlur)
	{
		BackgroundBlur->SetBlurStrength(0.0f);
	}

	if (StartButton)
	{
		StartButton->SetVisibility(ESlateVisibility::Hidden);
	}

	
}