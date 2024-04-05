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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	class UArrowComponent* WeaponMountRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	class UArrowComponent* WeaponMountLeft;

	UPROPERTY()
	class AWeaponBase* CurrentRightWeapon;

	UPROPERTY()
	class AWeaponBase* CurrentLeftWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player")
	class USphereComponent* InteractSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player")
	class UWidgetComponent* HealthBarComponent;

	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void Turn(float Amount);

	bool bIsFiring;

	void Interact();
	void HoldWeapon(class AWeaponBase* Weapon);
	void DropWeapon();
	void FireRightPressed();
	void FireLeftPressed();
	void FireRightReleased();
	void FireLeftReleased();
	void FireRight(bool Toggle);
	void FireLeft(bool Toggle);

	float MaxHealth = 100;
	float Health = MaxHealth;
	void DecreaseHealth();

	UFUNCTION()
	void OnWeaponFired();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class UFloatingPawnMovement* FloatingPawnMovement;

	UFUNCTION()
	void  OnCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnCollisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnCollisionBoxHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void UpdateHealthBar();

	float CurrentVelocity = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void PrintMessageOnScreen(FString Message);
};
