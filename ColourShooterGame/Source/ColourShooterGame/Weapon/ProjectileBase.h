// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

UENUM(BlueprintType)
enum class ERayColour :uint8 {
	RedRay = 0,
	BlueRay = 1
};

UCLASS()
class COLOURSHOOTERGAME_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	float Damage;

	UPROPERTY(EditDefaultsOnly)
	float Speed;

	UPROPERTY(EditDefaultsOnly)
	float LifeTime;

	UPROPERTY(EditAnywhere, Category = "RayColour")
	ERayColour rayColour;

protected:
	AProjectileBase();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnActorHit(AActor* Self, AActor* Other, FVector NormalImpulse, const FHitResult& Hit);

private:
	void DestroySelf();
	FTimerHandle Timer;
	void PrintMessageOnScreen(FString Message);

};
