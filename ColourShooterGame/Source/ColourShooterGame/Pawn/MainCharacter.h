// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class COLOURSHOOTERGAME_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player")
	class USphereComponent* InteractSphere;

	UPROPERTY(EditDefaultsOnly)
	float MaxWalkSpeed;

	UPROPERTY(EditDefaultsOnly)
	float MaxJogSpeed;

	UPROPERTY(EditDefaultsOnly)
	float MaxPickUpDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	class UArrowComponent* WeaponMountRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	class UArrowComponent* WeaponMountLeft;

	UPROPERTY()
	class AWeaponBase* CurrentRightWeapon;

	UPROPERTY()
	class AWeaponBase* CurrentLeftWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player")
	class UWidgetComponent* HealthBarComponent;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player")
	//TSubclassOf<class UPlayerHUD> WBP_PlayerHUD;

	//class UPlayerHUD* Hud;

	void Move(FVector Direction, float Scale);

	void FireRightPressed();
	void FireLeftPressed();
	void FireRightReleased();
	void FireLeftReleased();
	void FireRight(bool Toggle);
	void FireLeft(bool Toggle);

	bool bIsFiring;

	void Interact();
	void HoldWeapon(class AWeaponBase* Weapon);

	UFUNCTION()
	void OnWeaponFired();

	//class USkeletalMeshComponent* GetSkeletalMesh();

	UPROPERTY(EditAnywhere)
	float MaximumHealth = 100.0f;

	UPROPERTY(BlueprintReadOnly)
	float CurrentHealth;

	void DecreaseHealth();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void UpdateHealthBar();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	bool bMovementEnabled = false;

private:
	void UpdateMovement(float DeltaTime);

	void MoveHorizontal(float Scale);
	void MoveVertical(float Scale);

	void Turn(float Scale);

	class USkeletalMeshComponent* SkeletalMesh;
	class UCharacterBaseAnimation* AnimationInstance;

	UPROPERTY()
	class AWeaponBase* CurrentWeapon;

};
