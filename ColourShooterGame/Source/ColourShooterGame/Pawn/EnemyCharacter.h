// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class COLOURSHOOTERGAME_API AEnemyCharacter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy")
	class USphereComponent* CollisionSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemy")
	UStaticMeshComponent* VisualMesh;

	UPROPERTY(EditAnywhere)
	float MaximumHealth = 50.0f;

	UPROPERTY(BlueprintReadOnly)
	float CurrentHealth;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	class AMainPawn* MainPawn;

private:
	void PrintMessageOnScreen(FString Message);
};
