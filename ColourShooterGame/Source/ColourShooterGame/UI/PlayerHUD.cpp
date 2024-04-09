// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

#include "../Pawn/MainCharacter.h"
#include "../Weapon/WeaponBase.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Styling/SlateColor.h"

//UI HUD
void UPlayerHUD::NativeConstruct()
{
	if (ClickMeButton)
	{
		ClickMeButton->OnClicked.AddDynamic(this, &UPlayerHUD::ButtonClicked);
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

//UI HUD
void UPlayerHUD::ButtonClicked()
{
	//WeaponAmmoCountText->SetColorAndOpacity(FSlateColor(FLinearColor::Blue));

	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		PC->ConsoleCommand("RestartLevel");
	}

}