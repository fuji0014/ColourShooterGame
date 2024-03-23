// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

UCLASS()
class COLOURSHOOTERGAME_API AMainPlayerController : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainPlayerController();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player")
	class UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player")
	UStaticMeshComponent* VisualMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* MainCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* SpringArm;

	void MoveForward(float Amount);
	void MoveRight(float Amount);
	void Turn(float Amount);

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

};
