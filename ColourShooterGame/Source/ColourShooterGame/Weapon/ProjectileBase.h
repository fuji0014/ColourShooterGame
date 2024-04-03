// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

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

protected:
	AProjectileBase();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnActorHit(AActor* Self, AActor* Other, FVector NormalImpulse, const FHitResult& Hit);

private:
	void DestroySelf();

private:
	FTimerHandle Timer;
	void PrintMessageOnScreen(FString Message);

};
