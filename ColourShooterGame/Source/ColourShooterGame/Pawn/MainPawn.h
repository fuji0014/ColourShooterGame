// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainPawn.generated.h"


UCLASS()
class COLOURSHOOTERGAME_API AMainPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainPawn();


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player")
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player")
	UStaticMeshComponent* VisualMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* MainCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	class UArrowComponent* WeaponMount;

	UPROPERTY()
	class AWeaponBase* CurrentWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player")
	class USphereComponent* InteractSphere;

	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void Turn(float Amount);

	bool bIsFiring;

	void Interact();
	void HoldWeapon(class AWeaponBase* Weapon);
	void DropWeapon();
	void FirePressed();
	void FireReleased();
	void Fire(bool Toggle);

	float MaxHealth = 100;
	float Health = MaxHealth;
	void DecreaseHealth();

	UFUNCTION()
	void OnWeaponFired();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class UFloatingPawnMovement* FloatingPawnMovement;

	float CurrentVelocity = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	void PrintMessageOnScreen(FString Message);
};