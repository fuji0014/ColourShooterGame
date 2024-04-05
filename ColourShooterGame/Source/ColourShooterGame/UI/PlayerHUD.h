// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class COLOURSHOOTERGAME_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* WeaponAmmoCountText = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* ClickMeButton = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UProgressBar* HealthBar = nullptr;

	virtual bool Initialize() override;

	UFUNCTION()
	FText SetWeaponAmmoCount();

	UFUNCTION()
	float SetHealthProgress();

private:
	UFUNCTION()
	void ButtonClicked();
};
